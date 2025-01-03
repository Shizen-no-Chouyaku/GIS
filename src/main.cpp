#include "Utils/SDLUtils.h"
#include "Utils/MainLoop.h"
#include "Rendering/TileRenderer.h"
#include "UI/InputHandler.h"
#include "UI/MapWindow.h"
#include "UI/UIManager.h"
#include "UI/Toolbar.h"
#include "UI/LayerWindow.h"
#include "Utils/Utils.h"
#include <SDL2/SDL_ttf.h> // Include SDL_ttf

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

    // Create and add Toolbar
    std::shared_ptr<Toolbar> toolbar = std::make_shared<Toolbar>(renderer);

    std::shared_ptr<LayerWindow> layerWindow = std::make_shared<LayerWindow>(renderer);

    // Add MapWindow first
    uiManager.addComponent(mapWindow);
    
    // Add Toolbar after
    uiManager.addComponent(layerWindow);

    uiManager.addComponent(toolbar); // Toolbar renders on top


    // Run the main loop
    runMainLoop(window, renderer, uiManager, *mapWindow);

    // Cleanup and exit
    SDLUtils::cleanup(window, renderer);
    TTF_Quit(); // Quit SDL_ttf
    return 0;
}
