// main.cpp:

#include "Utils/SDLUtils.h"
#include "Rendering/MainLoop.h"
#include "Rendering/TileRenderer.h"
#include "UI/InputHandler.h"
#include "UI/Windows/MapWindow.h"
#include "UI/UIManager.h"
#include "UI/Windows/Toolbar.h"
#include "UI/Windows/LayerWindow.h"
#include "Config/ConfigManager.h" // Make sure to include ConfigManager
#include "UI//Windows/SettingsWindow.h"    // Include SettingsWindow if needed
#include "nlohmann/json.hpp"      // Include JSON library
#include <SDL2/SDL_ttf.h> // Include SDL_ttf
#include "Utils/Utils.h"

int main(int argc, char* argv[]) {
    // Initialize SDL and SDL_image
    if (!SDLUtils::initializeSDL()) {
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        Utils::logError("TTF_Init Error: " + std::string(TTF_GetError()));
        SDLUtils::cleanup(nullptr, nullptr);
        return 1;
    }

    // Create SDL window and renderer
    SDL_Window* window = SDLUtils::createWindow("Custom GIS", 1920 , 1080);
    if (!window) {
        TTF_Quit();
        SDLUtils::cleanup(nullptr, nullptr);
        return 1;
    }

    SDL_Renderer* renderer = SDLUtils::createRenderer(window);
    if (!renderer) {
        SDL_DestroyWindow(window);
        TTF_Quit();
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

    // Create LayerWindow
    std::shared_ptr<LayerWindow> layerWindow = std::make_shared<LayerWindow>(renderer);

    // **Add MapWindow and LayerWindow first**
    uiManager.addComponent(mapWindow);
    uiManager.addComponent(layerWindow);

    // **Create and add Toolbar after**
    std::shared_ptr<Toolbar> toolbar = std::make_shared<Toolbar>(renderer, uiManager);
    uiManager.addComponent(toolbar);

    // Run the main loop
    runMainLoop(window, renderer, uiManager, *mapWindow);

    // Cleanup and exit
    SDLUtils::cleanup(window, renderer);
    TTF_Quit(); // Quit SDL_ttf
    return 0;
}
