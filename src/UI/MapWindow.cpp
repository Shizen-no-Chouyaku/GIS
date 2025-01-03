// src/UI/MapWindow.cpp

#include "MapWindow.h"

MapWindow::MapWindow(TileRenderer& tileRenderer, InputHandler& inputHandler, SDL_Renderer* renderer)
    : tileRenderer(tileRenderer), inputHandler(inputHandler), renderer(renderer) {
    mapArea = {200, 30, 1720, 1050};  // x, y, width, height
}

MapWindow::~MapWindow() {}

void MapWindow::handleEvent(const SDL_Event& event) {
    inputHandler.handleEvent(event);
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
