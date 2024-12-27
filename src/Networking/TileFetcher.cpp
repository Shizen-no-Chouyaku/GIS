// src/Networking/TileFetcher.cpp

#include "TileFetcher.h"
#include "../Utils/Utils.h"
#include <curl/curl.h>
#include <sstream>
#include <fstream>

// Callback function for libcurl to write fetched data into a buffer
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* mem = static_cast<std::string*>(userp);
    mem->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

TileFetcher::TileFetcher(SDL_Renderer* renderer, size_t numThreads, size_t maxCacheSize)
    : renderer(renderer), maxCacheSize(maxCacheSize), threadPool(numThreads)
{
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

TileFetcher::~TileFetcher() {
    // Cleanup cached textures
    for(auto& pair : tileCache) {
        if(pair.second) {
            SDL_DestroyTexture(pair.second);
        }
    }
    curl_global_cleanup();
}

std::string TileFetcher::getTileURL(int z, int x, int y) {
    std::stringstream ss;
    ss << "https://tile.openstreetmap.org/" << z << "/" << x << "/" << y << ".png";
    return ss.str();
}

SDL_Texture* TileFetcher::fetchTileTask(int z, int x, int y) {
    TileKey key = {z, x, y};

    // Check cache first with shared (read) lock
    {
        std::shared_lock<std::shared_mutex> lock(cacheMutex);
        if(tileCache.find(key) != tileCache.end()) {
            // Move the accessed tile to the front of the LRU list
            // Upgrade to unique lock
            lock.unlock();
            std::unique_lock<std::shared_mutex> uniqueLock(cacheMutex);
            touchTile(key);
            Utils::logInfo("Tile found in memory cache: z:" + std::to_string(z) + " x:" + std::to_string(x) + " y:" + std::to_string(y));
            return tileCache[key];
        }
    }

    // Define cache path
    std::filesystem::path cacheDir = "resources/tiles/" + std::to_string(z) + "/" + std::to_string(x);
    std::filesystem::create_directories(cacheDir); // Creates directories if they don't exist
    std::filesystem::path cachePath = cacheDir / (std::to_string(y) + ".png");

    // Check if tile exists on disk
    if(std::filesystem::exists(cachePath)) {
        SDL_Surface* surface = IMG_Load(cachePath.string().c_str());
        if(surface) {
            // Convert Surface to RGBA32 to ensure compatibility
            SDL_Surface* convertedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
            if(!convertedSurface) {
                Utils::logError("SDL_ConvertSurfaceFormat Error for tile z:" + std::to_string(z) + " x:" + std::to_string(x) + " y:" + std::to_string(y) + ": " + std::string(SDL_GetError()));
                SDL_FreeSurface(surface);
                return nullptr;
            }

            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, convertedSurface);
            SDL_FreeSurface(convertedSurface);
            SDL_FreeSurface(surface);
            if(texture) {
                std::unique_lock<std::shared_mutex> lock(cacheMutex);
                tileCache[key] = texture;
                touchTile(key);
                evictIfNeeded();
                Utils::logInfo("Loaded tile from disk: " + cachePath.string());
                return texture;
            } else {
                Utils::logError("Failed to create texture from surface for tile: " + cachePath.string() + " Error: " + std::string(SDL_GetError()));
            }
        } else {
            Utils::logError("Failed to load tile from disk: " + cachePath.string() + " Error: " + std::string(IMG_GetError()));
        }
    }

    // Fetch tile from server
    CURL* curl = curl_easy_init();
    if(!curl) {
        Utils::logError("Failed to initialize CURL");
        return nullptr;
    }

    std::string url = getTileURL(z, x, y);
    Utils::logInfo("Fetching tile from server: " + url);
    std::string buffer;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "CustomGIS/1.0");

    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        Utils::logError("CURL error while fetching tile z:" + std::to_string(z) + " x:" + std::to_string(x) + " y:" + std::to_string(y) + ": " + std::string(curl_easy_strerror(res)));
        curl_easy_cleanup(curl);
        return nullptr;
    }

    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    if(response_code != 200) {
        Utils::logError("Failed to fetch tile z:" + std::to_string(z) + " x:" + std::to_string(x) + " y:" + std::to_string(y) + ": HTTP " + std::to_string(response_code));
        curl_easy_cleanup(curl);
        return nullptr;
    }

    curl_easy_cleanup(curl);

    // Load image from buffer
    SDL_RWops* rw = SDL_RWFromConstMem(buffer.data(), buffer.size());
    if(!rw) {
        Utils::logError("Failed to create RWops for tile z:" + std::to_string(z) + " x:" + std::to_string(x) + " y:" + std::to_string(y));
        return nullptr;
    }

    SDL_Surface* surface = IMG_LoadPNG_RW(rw);
    if(!surface) {
        Utils::logError("IMG_LoadPNG_RW Error for tile z:" + std::to_string(z) + " x:" + std::to_string(x) + " y:" + std::to_string(y) + ": " + std::string(IMG_GetError()));
        SDL_RWclose(rw);
        return nullptr;
    }

    // Convert Surface to RGBA32 to ensure compatibility
    SDL_Surface* convertedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    if(!convertedSurface) {
        Utils::logError("SDL_ConvertSurfaceFormat Error for tile z:" + std::to_string(z) + " x:" + std::to_string(x) + " y:" + std::to_string(y) + ": " + std::string(SDL_GetError()));
        SDL_FreeSurface(surface);
        SDL_RWclose(rw);
        return nullptr;
    }

    // Save tile to disk BEFORE freeing the original surface
    if(!std::filesystem::exists(cacheDir)) {
        std::filesystem::create_directories(cacheDir);
    }
    if(IMG_SavePNG(convertedSurface, cachePath.string().c_str()) != 0) {
        Utils::logError("Failed to save tile to disk: " + cachePath.string() + " Error: " + std::string(IMG_GetError()));
    } else {
        Utils::logInfo("Saved tile to disk: " + cachePath.string());
    }

    // Create texture from converted surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, convertedSurface);
    if(!texture) {
        Utils::logError("SDL_CreateTextureFromSurface Error for tile z:" + std::to_string(z) + " x:" + std::to_string(x) + " y:" + std::to_string(y) + ": " + std::string(SDL_GetError()));
    }

    SDL_FreeSurface(convertedSurface);
    SDL_FreeSurface(surface);
    SDL_RWclose(rw);

    if(texture) {
        std::unique_lock<std::shared_mutex> lock(cacheMutex);
        tileCache[key] = texture;
        touchTile(key);
        evictIfNeeded();
        Utils::logInfo("Fetched and cached tile z:" + std::to_string(z) + " x:" + std::to_string(x) + " y:" + std::to_string(y));
    }

    return texture;
}

std::future<SDL_Texture*> TileFetcher::fetchTile(int z, int x, int y) {
    return threadPool.enqueue(&TileFetcher::fetchTileTask, this, z, x, y);
}

// Public method to check if a tile is cached
bool TileFetcher::isTileCached(int z, int x, int y) {
    TileKey key = {z, x, y};
    std::shared_lock<std::shared_mutex> lock(cacheMutex);
    return tileCache.find(key) != tileCache.end();
}

// Public method to retrieve a cached tile
SDL_Texture* TileFetcher::getTile(int z, int x, int y) {
    TileKey key = {z, x, y};
    std::shared_lock<std::shared_mutex> lock(cacheMutex);
    if(tileCache.find(key) != tileCache.end()) {
        touchTile(key);
        return tileCache[key];
    }
    return nullptr;
}

// Move a key to the front of the LRU list
void TileFetcher::touchTile(const TileKey& key) {
    // If the key exists in cacheIterators, move it to the front
    if(cacheIterators.find(key) != cacheIterators.end()) {
        lruList.erase(cacheIterators[key]);
    }
    lruList.push_front(key);
    cacheIterators[key] = lruList.begin();
}

// Evict least recently used tile if cache exceeds max size
void TileFetcher::evictIfNeeded() {
    while(tileCache.size() > maxCacheSize) {
        // Remove from the back of the list
        TileKey lruKey = lruList.back();
        lruList.pop_back();

        // Erase from cache
        if(tileCache.find(lruKey) != tileCache.end()) {
            SDL_DestroyTexture(tileCache[lruKey]);
            tileCache.erase(lruKey);
        }

        // Erase from cacheIterators
        cacheIterators.erase(lruKey);

        Utils::logInfo("Evicted tile from cache: z:" + std::to_string(lruKey.z) +
                      " x:" + std::to_string(lruKey.x) +
                      " y:" + std::to_string(lruKey.y));
    }
}
