// src/Rendering/TileRenderer.h

#ifndef TILERENDERER_H
#define TILERENDERER_H

#include <vector>
#include <mutex>
#include <unordered_map> // Using unordered_map
#include <future>
#include <shared_mutex>
#include <utility> // For std::pair
#include <SDL2/SDL.h> // SDL types and functions
#include "../Networking/TileKey.h" // Shared TileKey definitions
#include "../Networking/TileFetcher.h" // Ensure TileFetcher is properly included
#include "Viewport.h" // Ensure Viewport is defined and included

class TileRenderer {
public:
    TileRenderer(SDL_Renderer* renderer);
    ~TileRenderer();

    void setViewport(const Viewport& vp);
    void render(const SDL_Rect& mapArea);

    // Additional public methods if necessary
    bool needsRedraw() const;
    void resetRedrawFlag();
    void updateTiles();

    Viewport viewport; // Made public for easy access

private:
    SDL_Renderer* renderer;
    TileFetcher tileFetcher;
    std::mutex renderMutex;

    // Use unordered_map instead of map since we have a hash function
    std::unordered_map<TileKey, std::future<SDL_Texture*>, TileKeyHash> tileFutures;

    bool needsRedrawFlag;

    // Precomputed tiles: pair of TileKey and destination rectangle
    std::vector<std::pair<TileKey, SDL_Rect>> precomputedTiles;

    // Helper functions
    std::pair<int, int> latLonToTile(double lat, double lon, int zoom);
    std::pair<double, double> tileToLatLon(int x, int y, int zoom);

    // Process completed tile futures
    void processTileFutures();

    // Precompute tile positions based on the current viewport
    void precomputeTilePositions();
};

#endif // TILERENDERER_H
