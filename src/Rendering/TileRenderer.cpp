#include "TileRenderer.h"
#include "../Utils/Utils.h"
#include <cmath>
#include <future>
#include <mutex>
#include <shared_mutex>
#include <SDL2/SDL.h>

TileRenderer::TileRenderer(SDL_Renderer* renderer)
    : renderer(renderer), 
      tileFetcher(renderer, 8 /* threads */, 1024 /* cacheSize */),
      needsRedrawFlag(true)
{
    viewport.centerLat = 35.6895; // tokyo coords 
    viewport.centerLon = 139.6917;
    viewport.zoom = 5;
    viewport.windowWidth = 800;
    viewport.windowHeight = 600;

    precomputeTilePositions();
}

TileRenderer::~TileRenderer() {
    Utils::logInfo("TileRenderer destroyed");
}

void TileRenderer::setViewport(const Viewport& vp) {
    std::lock_guard<std::mutex> lock(renderMutex);
    viewport = vp;
    needsRedrawFlag = true;
    tileFutures.clear();
    precomputeTilePositions();
}

bool TileRenderer::needsRedraw() const {
    return needsRedrawFlag;
}

void TileRenderer::resetRedrawFlag() {
    needsRedrawFlag = false;
}

std::pair<int, int> TileRenderer::latLonToTile(double lat, double lon, int zoom) {
    double latRad = lat * M_PI / 180.0;
    double n = pow(2.0, zoom);
    double x = (lon + 180.0) / 360.0 * n;
    double y = (1.0 - log(tan(latRad) + 1.0/cos(latRad)) / M_PI) / 2.0 * n;
    return { static_cast<int>(x), static_cast<int>(y) };
}

std::pair<double, double> TileRenderer::tileToLatLon(int x, int y, int zoom) {
    double n = pow(2.0, zoom);
    double lon = x / n * 360.0 - 180.0;
    double latRad = atan(sinh(M_PI * (1.0 - 2.0 * y / n)));
    double lat = latRad * 180.0 / M_PI;
    return { lat, lon };
}

void TileRenderer::processTileFutures() {
    bool tilesLoaded = false;
    for (auto it = tileFutures.begin(); it != tileFutures.end();) {
        std::future<SDL_Texture*>& fut = it->second;
        if (fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            SDL_Texture* texture = fut.get();
            if (texture) {
                tilesLoaded = true;
            } else {
                Utils::logError("Tile fetching failed for z:"
                                + std::to_string(it->first.z)
                                + " x:" + std::to_string(it->first.x)
                                + " y:" + std::to_string(it->first.y));
            }
            it = tileFutures.erase(it);
        } else {
            ++it;
        }
    }
    if (tilesLoaded) {
        needsRedrawFlag = true;
    }
}

void TileRenderer::updateTiles() {
    processTileFutures();
}

void TileRenderer::precomputeTilePositions() {
    precomputedTiles.clear();

    int z = viewport.zoom;
    double centerLat = viewport.centerLat;
    double centerLon = viewport.centerLon;
    int windowWidth = viewport.windowWidth;
    int windowHeight = viewport.windowHeight;

    double n = pow(2.0, z);
    double x = (centerLon + 180.0) / 360.0 * n;
    double y = (1.0 - log(tan(centerLat * M_PI/180.0)
             + 1.0/cos(centerLat * M_PI/180.0))/M_PI)/2.0 * n;

    double tileStartX = x - (windowWidth / 2.0) / 256.0;
    double tileStartY = y - (windowHeight / 2.0) / 256.0;

    int startTileX = static_cast<int>(floor(tileStartX));
    int startTileY = static_cast<int>(floor(tileStartY));

    double offsetX = (tileStartX - startTileX) * 256.0;
    double offsetY = (tileStartY - startTileY) * 256.0;

    int tilesX = static_cast<int>(ceil(double(windowWidth)/256.0)) + 2;
    int tilesY = static_cast<int>(ceil(double(windowHeight)/256.0)) + 2;

    for(int dx = 0; dx < tilesX; dx++){
        for(int dy = 0; dy < tilesY; dy++){
            int tileX = startTileX + dx;
            int tileY = startTileY + dy;

            int wrappedX = (tileX % static_cast<int>(n) + static_cast<int>(n)) 
                            % static_cast<int>(n);
            if(tileY < 0 || tileY >= static_cast<int>(n)) continue;

            TileKey key = {z, wrappedX, tileY};

            double screenX = dx*256.0 - offsetX;
            double screenY = dy*256.0 - offsetY;

            SDL_Rect dstRect = { 
                static_cast<int>(floor(screenX)), 
                static_cast<int>(floor(screenY)), 
                256, 256 
            };
            precomputedTiles.emplace_back(key, dstRect);
        }
    }
}

void TileRenderer::render() {
    std::lock_guard<std::mutex> lock(renderMutex);
    if(!needsRedrawFlag) {
        return;
    }
    needsRedrawFlag = false;

    // Clear background
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderClear(renderer);

    processTileFutures();

    // Collect tiles to render
    std::vector<std::pair<SDL_Texture*, SDL_Rect>> tilesToRender;
    for (auto& [key, dstRect] : precomputedTiles) {
        int z = key.z, x = key.x, y = key.y;

        SDL_Texture* tex = nullptr;
        if (tileFetcher.isTileCached(z, x, y)) {
            tex = tileFetcher.getTile(z, x, y);
        }

        if (!tex && tileFutures.find(key) == tileFutures.end()) {
            tileFutures[key] = tileFetcher.fetchTile(z, x, y);
        }

        if (tex) {
            tilesToRender.emplace_back(tex, dstRect);
        }
    }

    // Draw what we have
    for (auto& tile : tilesToRender) {
        SDL_Rect& dst = tile.second;
        SDL_RenderCopy(renderer, tile.first, nullptr, &dst);
    }

    // Optionally fill placeholders for missing tiles
    for(auto& [key, dstRect] : precomputedTiles) {
        if(!tileFetcher.isTileCached(key.z, key.x, key.y)
           && tileFutures.find(key) == tileFutures.end()) 
        {
            SDL_SetRenderDrawColor(renderer, 200,200,200,255);
            SDL_RenderFillRect(renderer, &dstRect);
        }
    }

    SDL_RenderPresent(renderer);
    Utils::logInfo("Render completed");
}
