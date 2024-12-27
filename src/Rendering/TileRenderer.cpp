// src/Rendering/TileRenderer.cpp

#include "TileRenderer.h"
#include "../Utils/Utils.h"
#include <cmath>        // For math functions
#include <future>
#include <mutex>
#include <shared_mutex>
#include <SDL2/SDL.h>    // For SDL functions

TileRenderer::TileRenderer(SDL_Renderer* renderer)
    : renderer(renderer), tileFetcher(renderer, 8), needsRedrawFlag(true)
{
    // Initialize default viewport
    viewport.centerLat = 51.5074; // Latitude for London
    viewport.centerLon = -0.1278; // Longitude for London
    viewport.zoom = 5;
    viewport.windowWidth = 800;
    viewport.windowHeight = 600;

    // Precompute tile positions initially
    precomputeTilePositions();
}

TileRenderer::~TileRenderer() {
    // Destructor implementation if necessary
}

void TileRenderer::setViewport(const Viewport& vp) {
    std::lock_guard<std::mutex> lock(renderMutex);
    viewport = vp;
    needsRedrawFlag = true;
    tileFutures.clear();
    precomputeTilePositions(); // Recompute tile positions
}

bool TileRenderer::needsRedraw() const {
    return needsRedrawFlag;
}

void TileRenderer::resetRedrawFlag() {
    needsRedrawFlag = false;
}

std::pair<int, int> TileRenderer::latLonToTile(double lat, double lon, int zoom) {
    double latRad = lat * M_PI / 180.0;
    double n = pow(2, zoom);
    double x = (lon + 180.0) / 360.0 * n;
    double y = (1.0 - log(tan(latRad) + 1.0 / cos(latRad)) / M_PI) / 2.0 * n;
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
    bool tilesLoaded = false; // Flag to check if any tile was loaded
    for(auto it = tileFutures.begin(); it != tileFutures.end(); ) {
        std::future<SDL_Texture*>& fut = it->second;
        if(fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            SDL_Texture* texture = fut.get();
            if(texture) {
                tilesLoaded = true; // At least one tile was loaded
            }
            else {
                Utils::logError("Tile fetching failed for tile z:" + std::to_string(it->first.z) +
                               " x:" + std::to_string(it->first.x) +
                               " y:" + std::to_string(it->first.y));
            }
            // Remove the future from the map
            it = tileFutures.erase(it);
        }
        else {
            ++it;
        }
    }
    if(tilesLoaded) {
        needsRedrawFlag = true; // Trigger a redraw if any tile was loaded
    }
}

void TileRenderer::precomputeTilePositions() {
    precomputedTiles.clear();

    int z = viewport.zoom;
    double centerLat = viewport.centerLat;
    double centerLon = viewport.centerLon;
    int windowWidth = viewport.windowWidth;
    int windowHeight = viewport.windowHeight;

    // Convert center latitude and longitude to fractional tile coordinates
    double n = pow(2.0, z);
    double x = (centerLon + 180.0) / 360.0 * n;
    double y = (1.0 - log(tan(centerLat * M_PI / 180.0) + 1.0 / cos(centerLat * M_PI / 180.0)) / M_PI) / 2.0 * n;

    // Determine the top-left tile indices
    double tileStartX = x - (static_cast<double>(windowWidth) / 2.0) / 256.0;
    double tileStartY = y - (static_cast<double>(windowHeight) / 2.0) / 256.0;

    int startTileX = static_cast<int>(floor(tileStartX));
    int startTileY = static_cast<int>(floor(tileStartY));

    // Calculate the pixel offset for the first tile
    double offsetX = (tileStartX - startTileX) * 256.0;
    double offsetY = (tileStartY - startTileY) * 256.0;

    // Number of tiles needed to cover the window
    int tilesX = static_cast<int>(ceil(static_cast<double>(windowWidth) / 256.0)) + 2;
    int tilesY = static_cast<int>(ceil(static_cast<double>(windowHeight) / 256.0)) + 2;

    for(int dx = 0; dx < tilesX; ++dx) {
        for(int dy = 0; dy < tilesY; ++dy) {
            int tileX = startTileX + dx;
            int tileY = startTileY + dy;

            int wrappedX = (tileX % static_cast<int>(n) + static_cast<int>(n)) % static_cast<int>(n);
            if(tileY < 0 || tileY >= static_cast<int>(n)) continue; // Ignore tiles outside the latitude bounds

            TileKey key = { z, wrappedX, tileY };

            // Calculate screen position with fractional offsets
            double screenX = dx * 256.0 - offsetX;
            double screenY = dy * 256.0 - offsetY;

            SDL_Rect dstRect = { static_cast<int>(floor(screenX)), static_cast<int>(floor(screenY)), 256, 256 };
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

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Process completed tile fetches
    processTileFutures();

    // Minimal logging
    Utils::logInfo("Render() called");

    std::vector<std::pair<SDL_Texture*, SDL_Rect>> tilesToRender;

    // Collect tiles to render
    for(auto& [key, dstRect] : precomputedTiles) {
        int z = key.z;
        int x = key.x;
        int y = key.y;

        SDL_Texture* texture = nullptr;

        if(tileFetcher.isTileCached(z, x, y)) {
            texture = tileFetcher.getTile(z, x, y);
        }

        if(!texture && tileFutures.find(key) == tileFutures.end()) {
            tileFutures[key] = tileFetcher.fetchTile(z, x, y);
        }

        if(texture) {
            tilesToRender.emplace_back(texture, dstRect);
        }
    }

    // Render all collected tiles
    for(auto& tile : tilesToRender) {
        // Check if the tile's rectangle is within the window bounds
        if(tile.second.x + tile.second.w < 0 || tile.second.x > viewport.windowWidth ||
           tile.second.y + tile.second.h < 0 || tile.second.y > viewport.windowHeight) {
            continue; // Skip rendering this tile
        }

        if(SDL_RenderCopy(renderer, tile.first, NULL, &tile.second) != 0) {
            Utils::logError("SDL_RenderCopy Error: " + std::string(SDL_GetError()));
        }
    }

    // Render placeholders for missing tiles
    for(auto& [key, dstRect] : precomputedTiles) {
        if(!tileFetcher.isTileCached(key.z, key.x, key.y) && tileFutures.find(key) == tileFutures.end()) {
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Light gray
            if(SDL_RenderFillRect(renderer, &dstRect) != 0) {
                Utils::logError("SDL_RenderFillRect Error: " + std::string(SDL_GetError()));
            }
        }
    }

    // Present the renderer
    SDL_RenderPresent(renderer);

    // Minimal logging
    Utils::logInfo("Render() completed");
}
