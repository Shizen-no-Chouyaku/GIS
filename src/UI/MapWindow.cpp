// src/UI/MapWindow.cpp

#include "MapWindow.h"

MapWindow::MapWindow(TileRenderer& tileRenderer, InputHandler& inputHandler, SDL_Renderer* renderer)
    : tileRenderer(tileRenderer), inputHandler(inputHandler), renderer(renderer) {
    mapArea = {200, 30, 1720, 1050};  // x, y, width, height
}

MapWindow::~MapWindow() {}

void MapWindow::handleEvent(const SDL_Event& event) {
    // Determine if the event is related to the map area
    bool isEventInMapArea = false;
    int x, y;
    
    if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
        x = event.button.x;
        y = event.button.y;
        isEventInMapArea = (x >= mapArea.x && x <= mapArea.x + mapArea.w &&
                            y >= mapArea.y && y <= mapArea.y + mapArea.h);
    }
    else if (event.type == SDL_MOUSEMOTION) {
        x = event.motion.x;
        y = event.motion.y;
        isEventInMapArea = (x >= mapArea.x && x <= mapArea.x + mapArea.w &&
                            y >= mapArea.y && y <= mapArea.y + mapArea.h);
    }
    
    if (isEventInMapArea) {
        inputHandler.handleEvent(event);
    }
    // Else, ignore the event
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
    // Assuming the toolbar height is 60 pixels
    int toolbarHeight = 30;
    setSize(newWidth, newHeight - toolbarHeight);
    setPosition(0, toolbarHeight);

    // Update the TileRenderer's viewport
    Viewport newViewport = tileRenderer.viewport;
    newViewport.windowWidth = newWidth;
    newViewport.windowHeight = newHeight - toolbarHeight;
    tileRenderer.setViewport(newViewport);
}
