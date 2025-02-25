#include "MapWindow.h"

MapWindow::MapWindow(TileRenderer& tileRenderer, InputHandler& inputHandler, SDL_Renderer* renderer)
    : tileRenderer(tileRenderer), inputHandler(inputHandler), renderer(renderer) {
    mapArea = {200, 30, 1720, 1050};  // x, y, width, height
}

MapWindow::~MapWindow() {}

bool MapWindow::handleEvent(const SDL_Event& event) {
    bool isEventInMapArea = false;
    int x = 0, y = 0;

    if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
        x = event.button.x;
        y = event.button.y;
    } 
    else if (event.type == SDL_MOUSEMOTION) {
        x = event.motion.x;
        y = event.motion.y;
    }
    // **Add This New Branch for Mouse Wheel**
    else if (event.type == SDL_MOUSEWHEEL) {
        // SDL_MOUSEWHEEL doesn't fill event.button.* or event.motion.*
        // so we must call SDL_GetMouseState
        SDL_GetMouseState(&x, &y);
    }

    // Check whether the mouse is within mapArea
    isEventInMapArea = (x >= mapArea.x && x < mapArea.x + mapArea.w &&
                        y >= mapArea.y && y < mapArea.y + mapArea.h);

    if (!isEventInMapArea) {
        return false; // Not in map area -> Not handled
    }
    
    // If we're here, the event was inside the map
    inputHandler.handleEvent(event);
    return true; // We have handled it
}

void MapWindow::update() {
    inputHandler.update();
    tileRenderer.updateTiles();
}

void MapWindow::render(SDL_Renderer* renderer) {
    tileRenderer.render(mapArea);
}

void MapWindow::setPosition(int x, int y) {
    mapArea.x = x;
    mapArea.y = y;
}

void MapWindow::setSize(int width, int height) {
    mapArea.w = width;
    mapArea.h = height;
}

bool MapWindow::needsRedraw() const {
    return tileRenderer.needsRedraw();
}

const SDL_Rect& MapWindow::getMapArea() const { // Modified to return a const reference
    return mapArea;
}

TileRenderer& MapWindow::getTileRenderer() {
    return tileRenderer;
}

void MapWindow::onWindowResize(int newWidth, int newHeight) {
    // Assuming the toolbar height is 30 pixels
    int toolbarHeight = 30;
    setSize(newWidth, newHeight - toolbarHeight);
    setPosition(0, toolbarHeight);

    // Update the TileRenderer's viewport
    Viewport newViewport = tileRenderer.viewport;
    newViewport.windowWidth = newWidth;
    newViewport.windowHeight = newHeight - toolbarHeight;
    tileRenderer.setViewport(newViewport);
}
