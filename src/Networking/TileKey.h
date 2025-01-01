// src/Networking/TileKey.h
#ifndef TILEKEY_H
#define TILEKEY_H

#include <functional>

struct TileKey {
    int z;
    int x;
    int y;

    bool operator==(const TileKey& other) const {
        return z == other.z && x == other.x && y == other.y;
    }
};

// Hash function for TileKey to be used in unordered_map
struct TileKeyHash {
    std::size_t operator()(const TileKey& key) const {
        return std::hash<int>()(key.z) ^ 
               (std::hash<int>()(key.x) << 1) ^ 
               (std::hash<int>()(key.y) << 2);
    }
};

#endif // TILEKEY_H
