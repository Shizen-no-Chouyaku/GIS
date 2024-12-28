#include "TileFetcher.h"
#include "../Utils/Utils.h"
#include <curl/curl.h>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include <iostream>     // Added for logging
#include <mutex>        // Added for additional mutexes if needed

// ------------------------------------------
// To rate limit fetching (need to reah osm policy to see if required)
// After each successful fetch, set this > 0.
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

TileFetcher::TileFetcher(SDL_Renderer* renderer, size_t numThreads, size_t maxCacheSize)
    : renderer(renderer), maxCacheSize(maxCacheSize), threadPool(numThreads)
{
    /* 
    Utils::logInfo("TileFetcher created with " + std::to_string(numThreads) 
                   + " threads, maxCacheSize=" + std::to_string(maxCacheSize));
    std::cout << "[TileFetcher] Constructor called. Renderer: " << renderer 
              << ", Threads: " << numThreads << ", MaxCacheSize: " << maxCacheSize << std::endl;
    */
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

TileFetcher::~TileFetcher() {
    // Cleanup cached textures
    // std::cout << "[TileFetcher] Destructor called. Cleaning up cached textures." << std::endl;
    for (auto& pair : tileCache) {
        if (pair.second) {
            SDL_DestroyTexture(pair.second);
            // std::cout << "[TileFetcher] Destroyed texture for tile z:" << pair.first.z 
                      // << " x:" << pair.first.x << " y:" << pair.first.y << std::endl;
        }
    }
    curl_global_cleanup();
    // Utils::logInfo("TileFetcher destroyed");
}

std::string TileFetcher::getTileURL(int z, int x, int y) {
    std::stringstream ss;
    ss << "https://tile.openstreetmap.org/" << z << "/" << x << "/" << y << ".png";
    return ss.str();
}

// The main tile fetching task
SDL_Texture* TileFetcher::fetchTileTask(int z, int x, int y) {
    // std::cout << "[fetchTileTask] BEGIN z:" << z << " x:" << x << " y:" << y 
              // << " [Thread " << std::this_thread::get_id() << "]" << std::endl;

    TileKey key = {z, x, y};

    // Avoid double fetch
    {
        // std::cout << "[fetchTileTask] Attempting to acquire exclusive lock on cacheMutex [Thread " 
        //          << std::this_thread::get_id() << "]." << std::endl;
        std::unique_lock<std::shared_mutex> lock(cacheMutex);
        // std::cout << "[fetchTileTask] Acquired exclusive lock on cacheMutex [Thread " 
        //         << std::this_thread::get_id() << "]." << std::endl;

        if (inProgressTiles.find(key) != inProgressTiles.end()) {
        //    std::cout << "[fetchTileTask] Tile is already being fetched by another thread [Thread " 
        //              << std::this_thread::get_id() << "]." << std::endl;
            return nullptr;
        }
        inProgressTiles.insert(key);
        // std::cout << "[fetchTileTask] Inserted tile into inProgressTiles [Thread " 
        //          << std::this_thread::get_id() << "]." << std::endl;
    }

    // Helper lambdas
    auto cleanup = [&]() {
        //std::cout << "[fetchTileTask] Cleaning up inProgressTiles for tile z:" << z 
        //          << " x:" << x << " y:" << y << " [Thread " << std::this_thread::get_id() << "]." << std::endl;
        std::unique_lock<std::shared_mutex> lock(cacheMutex);
        inProgressTiles.erase(key);
        //std::cout << "[fetchTileTask] Removed tile from inProgressTiles [Thread " 
        //         << std::this_thread::get_id() << "]." << std::endl;
    };
    auto failAndCleanup = [&]() -> SDL_Texture* {
        //std::cout << "[fetchTileTask] Failure detected. Calling failAndCleanup [Thread " 
        //          << std::this_thread::get_id() << "]." << std::endl;
        cleanup();
        return createPlaceholderTexture(); // so we can retry later
    };

    try {
        // 1) Check memory cache
        {
            //std::cout << "[fetchTileTask] Attempting to acquire shared lock on cacheMutex to check memory cache [Thread " 
            //          << std::this_thread::get_id() << "]." << std::endl;
            std::shared_lock<std::shared_mutex> lock(cacheMutex);
            //std::cout << "[fetchTileTask] Acquired shared lock on cacheMutex [Thread " 
            //          << std::this_thread::get_id() << "]." << std::endl;

            auto it = tileCache.find(key);
            if (it != tileCache.end()) {
                //std::cout << "[fetchTileTask] Tile found in memory cache. Releasing shared lock and updating LRU [Thread " 
                //          << std::this_thread::get_id() << "]." << std::endl;
                lock.unlock();
                std::unique_lock<std::shared_mutex> uniqueLock(cacheMutex);
                touchTile(key, uniqueLock);
                return it->second;
            }
            //std::cout << "[fetchTileTask] Tile not found in memory cache [Thread " 
            //          << std::this_thread::get_id() << "]." << std::endl;
        }

        // 2) Build disk cache path
        std::filesystem::path cacheDir =
            "resources/tiles/" + std::to_string(z) + "/" + std::to_string(x);
        std::filesystem::create_directories(cacheDir);
        std::filesystem::path cachePath = cacheDir / (std::to_string(y) + ".png");

        // 3) Check disk
        if (std::filesystem::exists(cachePath)) {
            //std::cout << "[fetchTileTask] Tile found on disk. Loading from disk [Thread " 
            //          << std::this_thread::get_id() << "]." << std::endl;
            SDL_Surface* surface = IMG_Load(cachePath.string().c_str());
            if (!surface) {
                std::cerr << "[fetchTileTask] Corrupted file detected. Removing from disk [Thread " 
                          << std::this_thread::get_id() << "]." << std::endl;
                std::filesystem::remove(cachePath);
                return failAndCleanup();
            }
            SDL_Surface* convSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
            SDL_FreeSurface(surface);

            if (!convSurface) {
                std::cerr << "[fetchTileTask] Failed to convert surface format [Thread " 
                          << std::this_thread::get_id() << "]." << std::endl;
                return failAndCleanup();
            }
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, convSurface);
            SDL_FreeSurface(convSurface);
            if (!texture) {
                std::cerr << "[fetchTileTask] Failed to create texture from surface [Thread " 
                          << std::this_thread::get_id() << "]." << std::endl;
                return failAndCleanup();
            }

            {
            //    std::cout << "[fetchTileTask] Attempting to acquire exclusive lock on cacheMutex to update memory cache [Thread " 
            //              << std::this_thread::get_id() << "]." << std::endl;
                std::unique_lock<std::shared_mutex> lock(cacheMutex);
            //    std::cout << "[fetchTileTask] Acquired exclusive lock on cacheMutex [Thread " 
            //              << std::this_thread::get_id() << "]." << std::endl;
                tileCache[key] = texture;
            //    std::cout << "[fetchTileTask] Updated memory cache with new texture [Thread " 
            //              << std::this_thread::get_id() << "]." << std::endl;
            }
            cleanup();
            return texture;
        }

        // 4) If not on disk, fetch from server
        //std::cout << "[fetchTileTask] Tile not found on disk. Fetching from server [Thread " 
        //          << std::this_thread::get_id() << "]." << std::endl;
        CURL* curl = curl_easy_init();
        if (!curl) {
            std::cerr << "[fetchTileTask] Failed to initialize cURL [Thread " 
                      << std::this_thread::get_id() << "]." << std::endl;
            return failAndCleanup();
        }

        std::string url = getTileURL(z, x, y);
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
            std::cerr << "[fetchTileTask] cURL perform failed: " << curl_easy_strerror(res) 
                      << " [Thread " << std::this_thread::get_id() << "]." << std::endl;
            curl_easy_cleanup(curl);
            return failAndCleanup();
        }
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_cleanup(curl);

        if (response_code != 200) {
            std::cerr << "[fetchTileTask] Received non-200 response code: " << response_code 
                      << " [Thread " << std::this_thread::get_id() << "]." << std::endl;
            return failAndCleanup();
        }

        // 5) Convert to SDL_Surface
        SDL_RWops* rw = SDL_RWFromConstMem(buffer.data(), static_cast<int>(buffer.size()));
        if (!rw) {
            std::cerr << "[fetchTileTask] Failed to create SDL_RWops from memory [Thread " 
                      << std::this_thread::get_id() << "]." << std::endl;
            return failAndCleanup();
        }
        SDL_Surface* surface = IMG_LoadPNG_RW(rw);
        SDL_RWclose(rw);
        if (!surface) {
            std::cerr << "[fetchTileTask] Failed to load PNG from memory [Thread " 
                      << std::this_thread::get_id() << "]." << std::endl;
            return failAndCleanup();
        }

        // 6) Save to disk using a .tmp approach to avoid partial writes
        static std::mutex saveMutex;
        std::filesystem::path tmpPath = cachePath.string() + ".tmp";
        {
            std::lock_guard<std::mutex> lock(saveMutex);
            //std::cout << "[fetchTileTask] Attempting to save tile to disk as tmp file [Thread " 
            //          << std::this_thread::get_id() << "]." << std::endl;
            if (IMG_SavePNG(surface, tmpPath.string().c_str()) != 0) {
                std::cerr << "[fetchTileTask] Failed to save PNG to tmp file [Thread " 
                          << std::this_thread::get_id() << "]." << std::endl;
                SDL_FreeSurface(surface);
                std::filesystem::remove(tmpPath);
                return failAndCleanup();
            }
            std::error_code ec;
            std::filesystem::rename(tmpPath, cachePath, ec);
            if (ec) {
                std::cerr << "[fetchTileTask] Failed to rename tmp file to final cache path: " 
                          << ec.message() << " [Thread " << std::this_thread::get_id() << "]." << std::endl;
                SDL_FreeSurface(surface);
                std::filesystem::remove(tmpPath);
                return failAndCleanup();
            }
            //std::cout << "[fetchTileTask] Successfully saved tile to disk [Thread " 
            //          << std::this_thread::get_id() << "]." << std::endl;
        }

        // 7) Create final texture
        SDL_Surface* convSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(surface);
        if (!convSurface) {
            std::cerr << "[fetchTileTask] Failed to convert surface format after loading from server [Thread " 
                      << std::this_thread::get_id() << "]." << std::endl;
            return failAndCleanup();
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, convSurface);
        SDL_FreeSurface(convSurface);
        if (!texture) {
            std::cerr << "[fetchTileTask] Failed to create texture from surface after loading from server [Thread " 
                      << std::this_thread::get_id() << "]." << std::endl;
            return failAndCleanup();
        }

        {
            //std::cout << "[fetchTileTask] Attempting to acquire exclusive lock on cacheMutex to update memory cache [Thread " 
            //          << std::this_thread::get_id() << "]." << std::endl;
            std::unique_lock<std::shared_mutex> lock(cacheMutex);
            //std::cout << "[fetchTileTask] Acquired exclusive lock on cacheMutex [Thread " 
            //          << std::this_thread::get_id() << "]." << std::endl;
            tileCache[key] = texture;
            //std::cout << "[fetchTileTask] Updated memory cache with new texture [Thread " 
            //          << std::this_thread::get_id() << "]." << std::endl;
        }

        if (WAIT_BETWEEN_FETCHES_MS > 0) {
            //std::cout << "[fetchTileTask] Waiting for " << WAIT_BETWEEN_FETCHES_MS 
            //          << " ms before next fetch [Thread " << std::this_thread::get_id() << "]." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_BETWEEN_FETCHES_MS));
        }

        cleanup();
        //std::cout << "[fetchTileTask] END   z:" << z << " x:" << x << " y:" << y 
        //          << " [Thread " << std::this_thread::get_id() << "]." << std::endl;
        return texture;
    } catch (const std::exception& e) {
        std::cerr << "[TileFetcher] Exception while fetching tile z:" << z
                  << " x:" << x << " y:" << y
                  << " => " << e.what() << " [Thread " << std::this_thread::get_id() << "]." << std::endl;
        cleanup();
        // Return a placeholder so we don't kill the worker thread:
        return createPlaceholderTexture();
    } catch (...) {
        std::cerr << "[TileFetcher] Unknown exception while fetching tile z:" << z
                  << " x:" << x << " y:" << y 
                  << " [Thread " << std::this_thread::get_id() << "]." << std::endl;
        cleanup();
        // Return a placeholder so we don't kill the worker thread:
        return createPlaceholderTexture();
    }

}

SDL_Texture* TileFetcher::createPlaceholderTexture() {
    //std::cout << "[createPlaceholderTexture] Creating placeholder texture [Thread " 
    //          << std::this_thread::get_id() << "]." << std::endl;
    SDL_Surface* placeholder = SDL_CreateRGBSurface(
        0, 256, 256, 32, 0x00FF0000,0x0000FF00,0x000000FF,0xFF000000);
    if (!placeholder) {
        std::cerr << "[createPlaceholderTexture] Failed to create placeholder surface [Thread " 
                  << std::this_thread::get_id() << "]." << std::endl;
        return nullptr;
    }
    SDL_FillRect(placeholder, nullptr, SDL_MapRGB(placeholder->format, 200,200,200));
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, placeholder);
    if (!tex) {
        std::cerr << "[createPlaceholderTexture] Failed to create placeholder texture from surface [Thread " 
                  << std::this_thread::get_id() << "]." << std::endl;
    } else {
    //    std::cout << "[createPlaceholderTexture] Successfully created placeholder texture [Thread " 
    //              << std::this_thread::get_id() << "]." << std::endl;
    }
    SDL_FreeSurface(placeholder);
    return tex;
}

std::future<SDL_Texture*> TileFetcher::fetchTile(int z, int x, int y) {
    //std::cout << "[fetchTile] Enqueuing tile fetch task for z:" << z << " x:" << x << " y:" << y 
    //          << " [Thread " << std::this_thread::get_id() << "]." << std::endl;
    return threadPool.enqueue(&TileFetcher::fetchTileTask, this, z, x, y);
}

// Check if tile is in memory
bool TileFetcher::isTileCached(int z, int x, int y) {
    TileKey key = {z, x, y};
    std::shared_lock<std::shared_mutex> lock(cacheMutex);
    //std::cout << "[isTileCached] Checking cache for tile z:" << z << " x:" << x << " y:" << y 
    //          << " [Thread " << std::this_thread::get_id() << "]." << std::endl;
    return (tileCache.find(key) != tileCache.end());
}

// Updated touchTile to accept a lock reference
void TileFetcher::touchTile(const TileKey& key, std::unique_lock<std::shared_mutex>& lock) {
    //std::cout << "[touchTile] Updating LRU for tile z:" << key.z << " x:" << key.x 
    //          << " y:" << key.y << " [Thread " << std::this_thread::get_id() << "]." << std::endl;
    // Move key to front (LRU)
    auto it = cacheIterators.find(key);
    if (it != cacheIterators.end()) {
        lruList.erase(it->second);
        //std::cout << "[touchTile] Erased old position of tile from LRU list [Thread " 
        //          << std::this_thread::get_id() << "]." << std::endl;
    }
    lruList.push_front(key);
    cacheIterators[key] = lruList.begin();
    //std::cout << "[touchTile] Inserted tile at front of LRU list [Thread " 
    //          << std::this_thread::get_id() << "]." << std::endl;
}

// Updated getTile
SDL_Texture* TileFetcher::getTile(int z, int x, int y) {
    TileKey key = {z, x, y};
    //std::cout << "[getTile] Attempting to acquire shared lock on cacheMutex [Thread " 
    //          << std::this_thread::get_id() << "]." << std::endl;
    std::shared_lock<std::shared_mutex> lock(cacheMutex);
    //std::cout << "[getTile] Acquired shared lock on cacheMutex [Thread " 
    //          << std::this_thread::get_id() << "]." << std::endl;

    auto it = tileCache.find(key);
    if (it != tileCache.end()) {
        //std::cout << "[getTile] Tile found in cache [Thread " 
        //          << std::this_thread::get_id() << "]." << std::endl;
        // Upgrade to unique_lock if necessary
        lock.unlock();
        //std::cout << "[getTile] Released shared lock on cacheMutex [Thread " 
        //          << std::this_thread::get_id() << "]. Attempting to acquire exclusive lock." << std::endl;
        std::unique_lock<std::shared_mutex> uniqueLock(cacheMutex);
        //std::cout << "[getTile] Acquired exclusive lock on cacheMutex [Thread " 
        //          << std::this_thread::get_id() << "]." << std::endl;
        touchTile(key, uniqueLock);
        //std::cout << "[getTile] Updated LRU for tile [Thread " 
        //          << std::this_thread::get_id() << "]." << std::endl;
        return it->second;
    }

    //std::cout << "[getTile] Tile not found in cache [Thread " 
    //          << std::this_thread::get_id() << "]." << std::endl;
    return nullptr;
}

void TileFetcher::evictIfNeeded() {
    //std::cout << "[evictIfNeeded] Checking if eviction is needed [Thread " 
    //          << std::this_thread::get_id() << "]." << std::endl;
    while (tileCache.size() > maxCacheSize) {
        TileKey lruKey = lruList.back();
        lruList.pop_back();
        auto it = tileCache.find(lruKey);
        if (it != tileCache.end()) {
            //std::cout << "[evictIfNeeded] Evicting tile z:" << lruKey.z << " x:" << lruKey.x 
            //          << " y:" << lruKey.y << " [Thread " << std::this_thread::get_id() << "]." << std::endl;
            SDL_DestroyTexture(it->second);
            tileCache.erase(it);
        }
        cacheIterators.erase(lruKey);
    }
}
