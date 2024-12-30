#ifndef TILERENDERER_H
#define TILERENDERER_H

#include <vector>
#include <mutex>
#include <unordered_map>
#include <future>
#include <SDL2/SDL.h>
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

private:
    SDL_Renderer* renderer;
    TileFetcher tileFetcher;
    std::mutex renderMutex;
    std::unordered_map<TileKey, std::future<SDL_Texture*>, TileKeyHash> tileFutures;
    bool needsRedrawFlag;
    std::vector<std::pair<TileKey, SDL_Rect>> precomputedTiles;

    std::pair<int, int> latLonToTile(double lat, double lon, int zoom);
    std::pair<double, double> tileToLatLon(int x, int y, int zoom);
    void processTileFutures();
    void precomputeTilePositions();
};

#endif // TILERENDERER_H
