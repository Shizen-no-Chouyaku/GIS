// src/main.cpp
#include "Utils/SDLUtils.h"
#include "Utils/MainLoop.h"
#include "Rendering/TileRenderer.h"
#include "UI/InputHandler.h"
#include "UI/MapWindow.h"
#include "UI/UIManager.h"
#include "UI/Toolbar.h" // Include other UI components as needed
#include "Utils/Utils.h"

int main(int argc, char* argv[]) {
    // Initialize SDL and SDL_image
    if (!SDLUtils::initializeSDL()) {
        return 1;
    }

    // Create SDL window and renderer
    SDL_Window* window = SDLUtils::createWindow("Custom GIS", 800, 600);
    if (!window) {
        SDLUtils::cleanup(nullptr, nullptr);
        return 1;
    }

    SDL_Renderer* renderer = SDLUtils::createRenderer(window);
    if (!renderer) {
        SDLUtils::cleanup(window, nullptr);
        return 1;
    }

    // Initialize TileRenderer and InputHandler
    TileRenderer tileRenderer(renderer);
    InputHandler inputHandler(tileRenderer);

    // Initialize UIManager
    UIManager uiManager(renderer);
    uiManager.setWindow(window); // Set the window pointer

    // Create MapWindow
    std::shared_ptr<MapWindow> mapWindow = std::make_shared<MapWindow>(tileRenderer, inputHandler, renderer);

    // Create and add Toolbar
    std::shared_ptr<UIComponent> toolbar = std::make_shared<Toolbar>(renderer);

    // Add MapWindow first
    uiManager.addComponent(mapWindow);

    // Add Toolbar after
    uiManager.addComponent(toolbar); // Toolbar renders on top

    // Initialize and add other UI components as needed
    // e.g., Sidebar, StatusBar, etc.

    // Run the main loop
    runMainLoop(window, renderer, uiManager, *mapWindow);

    // Cleanup and exit
    SDLUtils::cleanup(window, renderer);
    return 0;
}
