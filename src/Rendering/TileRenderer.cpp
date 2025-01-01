// src/Rendering/TileRenderer.cpp
#include "TileRenderer.h"
#include "../Utils/Utils.h"
#include <cmath>
#include <future>
#include <mutex>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

TileRenderer::TileRenderer(SDL_Renderer* renderer)
    : renderer(renderer), tileFetcher(8 /* threads */, 1024 /* cacheSize */), needsRedrawFlag(true) 
{
    viewport.centerLat = 35.6895; // Tokyo coords
    viewport.centerLon = 139.6917;
    viewport.zoom = 5;
    viewport.windowWidth = 800;
    viewport.windowHeight = 600;

    precomputeTilePositions();
}

TileRenderer::~TileRenderer() {
    // Cleanup cached textures
    for (auto& pair : tileTextures) {
        if (pair.second) {
            SDL_DestroyTexture(pair.second);
        }
    }
    Utils::logInfo("TileRenderer destroyed");
}

void TileRenderer::setViewport(const Viewport& vp) {
    std::lock_guard<std::mutex> lock(renderMutex);
    viewport = vp;
    needsRedrawFlag = true;
    precomputeTilePositions();
}

bool TileRenderer::needsRedraw() const {
    return needsRedrawFlag;
}

void TileRenderer::resetRedrawFlag() {
    needsRedrawFlag = false;
}

void TileRenderer::updateTiles() {
    std::lock_guard<std::mutex> lock(renderMutex);
    processTileFutures();
}

void TileRenderer::render(const SDL_Rect& mapArea) {
    std::lock_guard<std::mutex> lock(renderMutex);

    if (!needsRedrawFlag) {
        SDL_Log("Skipping render: no redraw needed");
        return;
    }

    needsRedrawFlag = false;

    SDL_Log("Rendering map area: {x:%d, y:%d, w:%d, h:%d}", 
            mapArea.x, mapArea.y, mapArea.w, mapArea.h);

    // Clear the map area only (do not touch the UI area)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderSetViewport(renderer, &mapArea);
    SDL_RenderClear(renderer);

    // Load textures for visible tiles
    int renderedTiles = 0;
    std::vector<std::pair<SDL_Texture*, SDL_Rect>> tilesToRender;

    for (auto& [key, dstRect] : precomputedTiles) {
        SDL_Texture* tex = nullptr;

        auto it = tileTextures.find(key);
        if (it != tileTextures.end()) {
            tex = it->second;
        } else {
            // Check if tile is cached and load texture
            if (tileFetcher.isTileCached(key.z, key.x, key.y)) {
                loadTexture(key);
                it = tileTextures.find(key);
                if (it != tileTextures.end()) {
                    tex = it->second;
                }
            }

            // If texture still not available, enqueue fetch
            if (!tex && tileFutures.find(key) == tileFutures.end()) {
                tileFutures[key] = tileFetcher.fetchTile(key.z, key.x, key.y);
            }
        }

        if (tex) {
            tilesToRender.emplace_back(tex, dstRect);
            renderedTiles++;
        }
    }

    SDL_Log("Tiles rendered this frame: %d", renderedTiles);

    for (auto& tile : tilesToRender) {
        SDL_Rect& dst = tile.second;
        SDL_RenderCopy(renderer, tile.first, nullptr, &dst);
    }

    SDL_RenderSetViewport(renderer, nullptr); // Reset to full window
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
    double y = (1.0 - log(tan(centerLat * M_PI / 180.0) + 1.0 / cos(centerLat * M_PI / 180.0)) / M_PI) / 2.0 * n;

    double tileStartX = x - (windowWidth / 2.0) / 256.0;
    double tileStartY = y - (windowHeight / 2.0) / 256.0;

    double offsetX = (tileStartX - floor(tileStartX)) * 256.0;
    double offsetY = (tileStartY - floor(tileStartY)) * 256.0;

    int startTileX = static_cast<int>(tileStartX);
    int startTileY = static_cast<int>(tileStartY);

    int tilesX = static_cast<int>(ceil(double(windowWidth) / 256.0)) + 2;
    int tilesY = static_cast<int>(ceil(double(windowHeight) / 256.0)) + 2;

    for (int dx = 0; dx < tilesX; dx++) {
        for (int dy = 0; dy < tilesY; dy++) {
            int tileX = startTileX + dx;
            int tileY = startTileY + dy;

            TileKey key = {z, tileX, tileY};

            double screenX = dx * 256.0 - offsetX;
            double screenY = dy * 256.0 - offsetY;

            SDL_Rect dstRect = {
                static_cast<int>(floor(screenX)),
                static_cast<int>(floor(screenY)),
                256, 256
            };
            precomputedTiles.emplace_back(key, dstRect);
        }
    }
}

void TileRenderer::processTileFutures() {
    int processedTiles = 0;
    int failedTiles = 0;

    for (auto it = tileFutures.begin(); it != tileFutures.end();) {
        std::future<bool>& fut = it->second;
        if (fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            bool success = fut.get();
            if (success) {
                loadTexture(it->first);
                processedTiles++;
            } else {
                failedTiles++;
                Utils::logError("Tile fetching failed for z:" + std::to_string(it->first.z) +
                                " x:" + std::to_string(it->first.x) +
                                " y:" + std::to_string(it->first.y));
            }
            it = tileFutures.erase(it);
        } else {
            ++it;
        }
    }

    if (processedTiles > 0 || failedTiles > 0) {
        SDL_Log("Processed tiles: %d, Failed tiles: %d", processedTiles, failedTiles);
    }

    if (processedTiles > 0) {
        needsRedrawFlag = true;
    }
}

void TileRenderer::loadTexture(const TileKey& key) {
    if (tileTextures.find(key) != tileTextures.end()) {
        return; // Texture already loaded
    }

    std::filesystem::path tilePath = tileFetcher.getTilePath(key.z, key.x, key.y);
    if (tilePath.empty()) {
        Utils::logError("Attempted to load texture for non-cached tile z=" +
                        std::to_string(key.z) + ", x=" + std::to_string(key.x) +
                        ", y=" + std::to_string(key.y));
        return;
    }

    SDL_Surface* surface = IMG_Load(tilePath.string().c_str());
    if (!surface) {
        Utils::logError("Failed to load image from " + tilePath.string() + " => " + IMG_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        Utils::logError("Failed to create texture from surface for tile z=" +
                        std::to_string(key.z) + ", x=" + std::to_string(key.x) +
                        ", y=" + std::to_string(key.y) + " => " + SDL_GetError());
        return;
    }

    tileTextures[key] = texture;
}

SDL_Texture* TileRenderer::createPlaceholderTexture() {
    SDL_Surface* placeholder = SDL_CreateRGBSurface(
        0, 256, 256, 32, 0x00FF0000,0x0000FF00,0x000000FF,0xFF000000);
    if (!placeholder) {
        Utils::logError("Failed to create placeholder surface => " + std::string(SDL_GetError()));
        return nullptr;
    }
    SDL_FillRect(placeholder, nullptr, SDL_MapRGB(placeholder->format, 200,200,200));
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, placeholder);
    if (!tex) {
        Utils::logError("Failed to create placeholder texture from surface => " + std::string(SDL_GetError()));
    }
    SDL_FreeSurface(placeholder);
    return tex;
}
