#include "MapWindow.h"

MapWindow::MapWindow(TileRenderer& tileRenderer, InputHandler& inputHandler, SDL_Renderer* renderer)
    : tileRenderer(tileRenderer), inputHandler(inputHandler), uiManager(renderer) {}

MapWindow::~MapWindow() {}

void MapWindow::handleEvent(const SDL_Event& event) {
    inputHandler.handleEvent(event);
    uiManager.handleEvent(event);
}

void MapWindow::update() {
    inputHandler.update();
    tileRenderer.updateTiles();
}

void MapWindow::render() {
    // Render the UI (static components like the toolbar)
    uiManager.render();

    // Define map rendering area (below the toolbar)
    SDL_Rect mapArea = {0, 60, 800, 540};
    tileRenderer.render(mapArea); // Render the map in the defined area
}

