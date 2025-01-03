// src/Rendering/TileRenderer.cpp

#include "TileRenderer.h"
#include "../Utils/Utils.h"
#include <cmath>
#include <future>
#include <mutex>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>

TileRenderer::TileRenderer(SDL_Renderer* renderer)
    : renderer(renderer), tileFetcher(8 /* threads */, 1024 /* cacheSize */), needsRedrawFlag(true) 
{
    viewport.centerLat = 35.6895; // Example: Tokyo coordinates
    viewport.centerLon = 139.6917;
    viewport.zoom = 6;
    viewport.windowWidth = 1920;
    viewport.windowHeight = 1080;

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
    
    // Clamp the zoom level within MIN_ZOOM and MAX_ZOOM
    int clampedZoom = std::clamp(vp.zoom, Viewport::MIN_ZOOM, Viewport::MAX_ZOOM);
    if (clampedZoom != vp.zoom) {
        Utils::logInfo("Zoom level clamped from " + std::to_string(vp.zoom) + " to " + std::to_string(clampedZoom));
    }

    // Update the viewport with the clamped zoom level
    Viewport updatedVp = vp;
    updatedVp.zoom = clampedZoom;

    viewport = updatedVp;
    needsRedrawFlag = true;
    precomputeTilePositions(); // Automatically called within setViewport
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

TileKey TileRenderer::getParentTile(const TileKey& key) const {
    if (key.z == 0) {
        return key; // No parent exists for zoom level 0
    }
    TileKey parent = { key.z - 1, key.x / 2, key.y / 2 };
    return parent;
}

void TileRenderer::renderParentTile(const TileKey& childKey, const SDL_Rect& dstRect) {
    TileKey parentKey = getParentTile(childKey);
    auto parentIt = tileTextures.find(parentKey);

    if (parentIt == tileTextures.end()) {
        // Parent tile not loaded; optionally, you can queue its loading
        if (tileFetcher.isTileCached(parentKey.z, parentKey.x, parentKey.y) &&
            tileFutures.find(parentKey) == tileFutures.end()) {
            tileFutures[parentKey] = tileFetcher.fetchTile(parentKey.z, parentKey.x, parentKey.y);
        }
        return; // Cannot render parent tile
    }

    SDL_Texture* parentTexture = parentIt->second;
    if (!parentTexture) {
        return; // Parent texture invalid
    }

    // Determine which quadrant the child tile is in the parent tile
    int quadrantX = childKey.x % 2;
    int quadrantY = childKey.y % 2;

    SDL_Rect srcRect;
    srcRect.x = quadrantX * 128; // Each quadrant is 128x128 in a 256x256 parent tile
    srcRect.y = quadrantY * 128;
    srcRect.w = 128;
    srcRect.h = 128;

    // Destination rectangle is the size of the child tile
    SDL_Rect scaledDstRect = dstRect;
    scaledDstRect.w = 256;
    scaledDstRect.h = 256;

    // Render the quadrant scaled up to fill the child tile's area
    // Use nearest-neighbor scaling to preserve pixel art (optional)
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_RenderCopy(renderer, parentTexture, &srcRect, &scaledDstRect);
}

void TileRenderer::render(const SDL_Rect& mapArea) {
    std::lock_guard<std::mutex> lock(renderMutex);

    if (!needsRedrawFlag) {
    //    SDL_Log("Skipping render: no redraw needed");
        return;
    }

    needsRedrawFlag = false;

    //SDL_Log("Rendering map area: {x:%d, y:%d, w:%d, h:%d}", 
    //        mapArea.x, mapArea.y, mapArea.w, mapArea.h);

    // Clear the map area only (do not touch the UI area)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
    SDL_RenderSetViewport(renderer, &mapArea);
    SDL_RenderClear(renderer);

    // Prepare to collect tiles to render
    int renderedTiles = 0;
    std::vector<std::pair<SDL_Texture*, SDL_Rect>> tilesToRender;
    std::vector<std::pair<TileKey, SDL_Rect>> parentTilesToRender;

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
        } else {
            // Attempt to render parent tile as placeholder
            parentTilesToRender.emplace_back(key, dstRect);
        }
    }

    //SDL_Log("Tiles rendered this frame: %d", renderedTiles);

    // Render available tiles
    for (auto& tile : tilesToRender) {
        SDL_Rect& dst = tile.second;
        SDL_RenderCopy(renderer, tile.first, nullptr, &dst);
    }

    // Render parent tiles as placeholders
    for (auto& [key, dstRect] : parentTilesToRender) {
        renderParentTile(key, dstRect);
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
            int rawTileX = startTileX + dx;
            int rawTileY = startTileY + dy;

            // Normalize tileX by wrapping around
            int tileX = rawTileX % static_cast<int>(n);
            if (tileX < 0) {
                tileX += static_cast<int>(n);
            }

            // Clamp tileY between 0 and (n - 1)
            int tileY = rawTileY;
            if (tileY < 0) {
                tileY = 0;
            } else if (tileY >= static_cast<int>(n)) {
                tileY = static_cast<int>(n) - 1;
            }

            TileKey key = { z, tileX, tileY };

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
        //SDL_Log("Processed tiles: %d, Failed tiles: %d", processedTiles, failedTiles);
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
        //Utils::logError("Attempted to load texture for non-cached tile z=" +
        //                std::to_string(key.z) + ", x=" + std::to_string(key.x) +
        //                ", y=" + std::to_string(key.y));
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
    // Fill with a gray color instead of white
    SDL_FillRect(placeholder, nullptr, SDL_MapRGB(placeholder->format, 200, 200, 200));
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, placeholder);
    if (!tex) {
        Utils::logError("Failed to create placeholder texture from surface => " + std::string(SDL_GetError()));
    }
    SDL_FreeSurface(placeholder);
    return tex;
}

double TileRenderer::getMaxLatitudeDelta() const {
    // Calculate the latitude span visible in the window using Web Mercator projection
    // Reference: https://en.wikipedia.org/wiki/Web_Mercator_projection#Mathematics

    // Constants
    constexpr double MAX_LATITUDE = 85.0511;
    constexpr double MIN_LATITUDE = -85.0511;

    // Convert center latitude to radians
    double centerLatRad = viewport.centerLat * M_PI / 180.0;

    // Calculate the Mercator Y for the center latitude
    double mercatorYCenter = log(tan(M_PI / 4.0 + centerLatRad / 2.0));

    // Calculate the total map height in pixels at current zoom
    double mapHeight = 256.0 * pow(2.0, viewport.zoom);

    // Pixels per Mercator unit
    double pixelsPerMercatorUnit = mapHeight / (2.0 * M_PI);

    // Calculate how many Mercator units correspond to half the window height
    double mercatorHalfSpan = (viewport.windowHeight / 2.0) / pixelsPerMercatorUnit;

    // Calculate the Mercator Y bounds
    double mercatorYTop = mercatorYCenter + mercatorHalfSpan;
    double mercatorYBottom = mercatorYCenter - mercatorHalfSpan;

    // Convert Mercator Y bounds back to latitude
    auto inverseMercatorY = [](double y) -> double {
        return (2.0 * atan(exp(y)) - M_PI / 2.0) * 180.0 / M_PI;
    };

    double topLat = inverseMercatorY(mercatorYTop);
    double bottomLat = inverseMercatorY(mercatorYBottom);

    // Calculate delta from center to top or bottom
    double deltaLatTop = topLat - viewport.centerLat;
    double deltaLatBottom = viewport.centerLat - bottomLat;

    // The maximum delta is the smaller of the two
    double maxDeltaLat = std::min(deltaLatTop, deltaLatBottom);

    return maxDeltaLat;
}
