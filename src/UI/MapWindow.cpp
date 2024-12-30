#include "MapWindow.h"

MapWindow::MapWindow(TileRenderer& tileRenderer, InputHandler& inputHandler, SDL_Renderer* renderer)
    : tileRenderer(tileRenderer), inputHandler(inputHandler), renderer(renderer) {
    mapArea = {0, 60, 800, 540}; // Example values
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
