// src/Rendering/TileRenderer.h
#ifndef TILERENDERER_H
#define TILERENDERER_H

#include <vector>
#include <mutex>
#include <unordered_map>
#include <future>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Networking/TileKey.h"
#include "../Networking/TileFetcher.h"
#include "Viewport.h"

class TileRenderer {
public:
    TileRenderer(SDL_Renderer* renderer);
    ~TileRenderer();

    void setViewport(const Viewport& vp);
    void render(const SDL_Rect& mapArea);

    bool needsRedraw() const;
    void resetRedrawFlag();
    void updateTiles();

    Viewport viewport;

    // New method to get maximum latitude delta
    double getMaxLatitudeDelta() const;

private:
    SDL_Renderer* renderer;
    TileFetcher tileFetcher;
    std::mutex renderMutex;
    std::unordered_map<TileKey, SDL_Texture*, TileKeyHash> tileTextures;
    std::unordered_map<TileKey, std::future<bool>, TileKeyHash> tileFutures;
    bool needsRedrawFlag;
    std::vector<std::pair<TileKey, SDL_Rect>> precomputedTiles;

    void processTileFutures();
    void precomputeTilePositions();
    void loadTexture(const TileKey& key);
    SDL_Texture* createPlaceholderTexture();

    // New helper functions
    TileKey getParentTile(const TileKey& key) const;
    void renderParentTile(const TileKey& childKey, const SDL_Rect& dstRect);
};

#endif // TILERENDERER_H
