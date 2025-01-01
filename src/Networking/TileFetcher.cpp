// src/Networking/TileFetcher.cpp
#include "TileFetcher.h"
#include "../Utils/Utils.h"
#include <curl/curl.h>
#include <fstream>
#include <iostream>

// ------------------------------------------
// Rate limiting fetching as per OSM policy
// ------------------------------------------
static const int WAIT_BETWEEN_FETCHES_MS = 100;

// Timeout settings (prevents cURL from hanging forever)
static const long TIMEOUT_SECONDS = 30L;         // total transfer timeout
static const long CONNECT_TIMEOUT_SECONDS = 10L; // time allowed to connect

// Callback for libcurl to write fetched data into a buffer
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* mem = static_cast<std::string*>(userp);
    mem->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

TileFetcher::TileFetcher(size_t numThreads, size_t maxCacheSize)
    : maxCacheSize(maxCacheSize), threadPool(numThreads)
{
    Utils::logInfo("TileFetcher created with " + std::to_string(numThreads) 
                   + " threads, maxCacheSize=" + std::to_string(maxCacheSize));
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

TileFetcher::~TileFetcher() {
    Utils::logInfo("TileFetcher destroyed. Cleaning up cached tiles.");
    curl_global_cleanup();
}

std::string TileFetcher::getTileURL(int z, int x, int y) {
    std::stringstream ss;
    ss << "https://tile.openstreetmap.org/" << z << "/" << x << "/" << y << ".png";
    return ss.str();
}

bool TileFetcher::fetchTileTask(int z, int x, int y) {
    TileKey key = {z, x, y};

    Utils::logInfo("Starting fetchTileTask for z=" + std::to_string(z) +
                  ", x=" + std::to_string(x) + ", y=" + std::to_string(y));

    // Step 1: Acquire lock to check and insert into inProgressTiles
    {
        std::unique_lock<std::shared_mutex> lock(cacheMutex);
        if (inProgressTiles.find(key) != inProgressTiles.end()) {
            Utils::logInfo("Tile already in progress: z=" + std::to_string(z) +
                          ", x=" + std::to_string(x) + ", y=" + std::to_string(y));
            return false;
        }
        inProgressTiles.insert(key);
        Utils::logInfo("Inserted tile into inProgressTiles: z=" + std::to_string(z) +
                      ", x=" + std::to_string(x) + ", y=" + std::to_string(y));
    } // Release lock

    bool success = false;

    try {
        // Step 2: Check disk cache
        std::filesystem::path cacheDir =
            "resources/tiles/" + std::to_string(z) + "/" + std::to_string(x);
        std::filesystem::create_directories(cacheDir);
        std::filesystem::path cachePath = cacheDir / (std::to_string(y) + ".png");

        if (std::filesystem::exists(cachePath)) {
            Utils::logInfo("Tile found on disk: " + cachePath.string());

            // Update cache with the found tile
            {
                std::unique_lock<std::shared_mutex> lock(cacheMutex);
                tileCache[key] = cachePath;
                touchTile(key, lock);
                evictIfNeeded();
                Utils::logInfo("Updated cache with tile from disk: z=" + std::to_string(z) +
                              ", x=" + std::to_string(x) + ", y=" + std::to_string(y));
            }

            success = true;
            goto cleanup;
        }

        // Step 3: Fetch from server using cURL
        CURL* curl = curl_easy_init();
        if (!curl) {
            Utils::logError("Failed to initialize cURL for tile z=" + std::to_string(z) +
                            ", x=" + std::to_string(x) + ", y=" + std::to_string(y));
            goto cleanup;
        }

        std::string url = getTileURL(z, x, y);
        Utils::logInfo("Fetching tile from URL: " + url);
        std::string buffer;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "CustomGIS/1.0");

        // Timeouts
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT_SECONDS);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONNECT_TIMEOUT_SECONDS);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            Utils::logError("cURL perform failed for tile z=" + std::to_string(z) +
                            ", x=" + std::to_string(x) + ", y=" + std::to_string(y) +
                            " => " + curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            goto cleanup;
        }

        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_cleanup(curl);

        if (response_code != 200) {
            Utils::logError("Received non-200 response code (" + std::to_string(response_code) +
                            ") for tile z=" + std::to_string(z) +
                            ", x=" + std::to_string(x) + ", y=" + std::to_string(y));
            goto cleanup;
        }

        // Step 4: Save to disk using a .tmp approach to avoid partial writes
        std::filesystem::path tmpPath = cachePath.string() + ".tmp";
        {
            std::ofstream outFile(tmpPath, std::ios::binary);
            if (!outFile) {
                Utils::logError("Failed to open tmp file for writing: " + tmpPath.string());
                goto cleanup;
            }
            outFile.write(buffer.data(), buffer.size());
            if (!outFile) {
                Utils::logError("Failed to write data to tmp file: " + tmpPath.string());
                outFile.close();
                std::filesystem::remove(tmpPath);
                goto cleanup;
            }
            outFile.close();

            std::error_code ec;
            std::filesystem::rename(tmpPath, cachePath, ec);
            if (ec) {
                Utils::logError("Failed to rename tmp file to final cache path: " + ec.message());
                std::filesystem::remove(tmpPath);
                goto cleanup;
            }
        }

        // Step 5: Update cache with the newly fetched tile
        {
            std::unique_lock<std::shared_mutex> lock(cacheMutex);
            tileCache[key] = cachePath;
            touchTile(key, lock);
            evictIfNeeded();
            Utils::logInfo("Fetched and cached tile: z=" + std::to_string(z) +
                          ", x=" + std::to_string(x) + ", y=" + std::to_string(y));
        }

        if (WAIT_BETWEEN_FETCHES_MS > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_BETWEEN_FETCHES_MS));
        }

        success = true;
    } catch (const std::exception& e) {
        Utils::logError("Exception while fetching tile z=" + std::to_string(key.z) +
                        ", x=" + std::to_string(key.x) + ", y=" + std::to_string(key.y) +
                        " => " + e.what());
    } catch (...) {
        Utils::logError("Unknown exception while fetching tile z=" + std::to_string(key.z) +
                        ", x=" + std::to_string(key.x) + ", y=" + std::to_string(key.y));
    }

cleanup:
    // Step 6: Remove from inProgressTiles regardless of success or failure
    {
        std::unique_lock<std::shared_mutex> lock(cacheMutex);
        inProgressTiles.erase(key);
        Utils::logInfo("Removed tile from inProgressTiles: z=" + std::to_string(z) +
                      ", x=" + std::to_string(x) + ", y=" + std::to_string(y));
    }

    return success;
}

std::future<bool> TileFetcher::fetchTile(int z, int x, int y) {
    return threadPool.enqueue(&TileFetcher::fetchTileTask, this, z, x, y);
}

bool TileFetcher::isTileCached(int z, int x, int y) {
    TileKey key = {z, x, y};
    std::shared_lock<std::shared_mutex> lock(cacheMutex);
    return (tileCache.find(key) != tileCache.end());
}

std::filesystem::path TileFetcher::getTilePath(int z, int x, int y) {
    TileKey key = {z, x, y};
    std::shared_lock<std::shared_mutex> lock(cacheMutex);
    auto it = tileCache.find(key);
    if (it != tileCache.end()) {
        return it->second;
    }
    return ""; // Return empty path if not cached
}

void TileFetcher::touchTile(const TileKey& key, std::unique_lock<std::shared_mutex>& lock) {
    // Move key to front (LRU)
    auto it = cacheIterators.find(key);
    if (it != cacheIterators.end()) {
        lruList.erase(it->second);
    }
    lruList.push_front(key);
    cacheIterators[key] = lruList.begin();
}

void TileFetcher::evictIfNeeded() {
    while (tileCache.size() > maxCacheSize) {
        TileKey lruKey = lruList.back();
        lruList.pop_back();
        auto it = tileCache.find(lruKey);
        if (it != tileCache.end()) {
            Utils::logInfo("Evicting tile from cache: z=" + std::to_string(lruKey.z) +
                          ", x=" + std::to_string(lruKey.x) + ", y=" + std::to_string(lruKey.y));
            tileCache.erase(it);
        }
        cacheIterators.erase(lruKey);
    }
}
