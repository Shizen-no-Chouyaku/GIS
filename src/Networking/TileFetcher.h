// src/Networking/TileFetcher.h
#ifndef TILEFETCHER_H
#define TILEFETCHER_H

#include <string>
#include <unordered_map>
#include <list>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <shared_mutex>
#include <functional>
#include <future>
#include <tuple>
#include <filesystem>
#include "../Utils/ThreadPool.h"
#include "TileKey.h" // Shared TileKey definitions
#include <unordered_set>

class TileFetcher {
public:
    TileFetcher(SDL_Renderer* renderer, size_t numThreads = 4, size_t maxCacheSize = 200);
    ~TileFetcher();

    // Fetches a tile asynchronously; returns a future to the SDL_Texture*
    std::future<SDL_Texture*> fetchTile(int z, int x, int y);

    // Public methods to check cache and retrieve tiles
    bool isTileCached(int z, int x, int y);
    SDL_Texture* getTile(int z, int x, int y);

private:
    SDL_Renderer* renderer;
    size_t maxCacheSize;

    // LRU Cache using list and unordered_map
    std::list<TileKey> lruList; // Stores keys in order of usage
    std::unordered_map<TileKey, SDL_Texture*, TileKeyHash> tileCache;
    std::unordered_map<TileKey, std::list<TileKey>::iterator, TileKeyHash> cacheIterators;
    std::unordered_set<TileKey, TileKeyHash> inProgressTiles;
    
    mutable std::shared_mutex cacheMutex; // For concurrent reads

    ThreadPool threadPool;

    // Helper function to construct tile URL
    std::string getTileURL(int z, int x, int y);

    // Fetch tile task
    SDL_Texture* fetchTileTask(int z, int x, int y);
    SDL_Texture* createPlaceholderTexture();

    // Helper method to move a key to the front of the LRU list
    void touchTile(const TileKey& key, std::unique_lock<std::shared_mutex>& lock);

    // Evict least recently used tile
    void evictIfNeeded();
};

#endif // TILEFETCHER_H
