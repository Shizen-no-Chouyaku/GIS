#include "MapWindow.h"

MapWindow::MapWindow(TileRenderer& tileRenderer, InputHandler& inputHandler)
    : tileRenderer(tileRenderer), inputHandler(inputHandler) {}

MapWindow::~MapWindow() {}

void MapWindow::handleEvent(const SDL_Event& event) {
    inputHandler.handleEvent(event);
}

void MapWindow::update() {
    inputHandler.update();
    tileRenderer.updateTiles();
}

void MapWindow::render() {
    tileRenderer.render();
}
