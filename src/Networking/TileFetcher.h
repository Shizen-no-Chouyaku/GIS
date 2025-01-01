// src/Networking/TileFetcher.h
#ifndef TILEFETCHER_H
#define TILEFETCHER_H

#include <string>
#include <unordered_map>
#include <list>
#include <future>
#include <filesystem>
#include <mutex>
#include <shared_mutex>
#include <unordered_set>
#include "../Utils/ThreadPool.h"
#include "TileKey.h" // Shared TileKey definitions

class TileFetcher {
public:
    TileFetcher(size_t numThreads = 4, size_t maxCacheSize = 200);
    ~TileFetcher();

    // Fetches a tile asynchronously; returns a future indicating success or failure
    std::future<bool> fetchTile(int z, int x, int y);

    // Public methods to check cache
    bool isTileCached(int z, int x, int y);
    
    // Retrieves the file path of the cached tile
    std::filesystem::path getTilePath(int z, int x, int y);

private:
    size_t maxCacheSize;

    // LRU Cache using list and unordered_map
    std::list<TileKey> lruList; // Stores keys in order of usage
    std::unordered_map<TileKey, std::filesystem::path, TileKeyHash> tileCache;
    std::unordered_map<TileKey, std::list<TileKey>::iterator, TileKeyHash> cacheIterators;
    std::unordered_set<TileKey, TileKeyHash> inProgressTiles;
    
    mutable std::shared_mutex cacheMutex; // For concurrent reads

    ThreadPool threadPool;

    // Helper function to construct tile URL
    std::string getTileURL(int z, int x, int y);

    // Fetch tile task
    bool fetchTileTask(int z, int x, int y);

    // Helper method to move a key to the front of the LRU list
    void touchTile(const TileKey& key, std::unique_lock<std::shared_mutex>& lock);

    // Evict least recently used tile
    void evictIfNeeded();
};

#endif // TILEFETCHER_H
