#include "Utils/SDLUtils.h"
#include "Utils/MainLoop.h"
#include "Rendering/TileRenderer.h"
#include "UI/InputHandler.h"
#include "UI/MapWindow.h"
#include "Utils/Utils.h"

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (!SDLUtils::initializeSDL()) {
        return 1;
    }

    // Create SDL window and renderer
    SDL_Window* window = SDLUtils::createWindow("Custom GIS", 800, 600);
    if (!window) {
        //Utils::logError("Window creation failed.");
        SDLUtils::cleanup(nullptr, nullptr);
        return 1;
    }
    else {
    //Utils::logInfo("Window created successfully.");
}

    SDL_Renderer* renderer = SDLUtils::createRenderer(window);
    if (!renderer) {
        //Utils::logError("Renderer creation failed.");
        SDLUtils::cleanup(window, nullptr);
        return 1;
    }
    else {
    //Utils::logInfo("Renderer created successfully.");
}

    // Initialize TileRenderer and InputHandler
    TileRenderer tileRenderer(renderer);
    InputHandler inputHandler(tileRenderer);

    // Initialize MapWindow with TileRenderer and InputHandler
    MapWindow mapWindow(tileRenderer, inputHandler, renderer);
    runMainLoop(window, renderer, mapWindow);


    // Run the main loop with MapWindow
    runMainLoop(window, renderer, mapWindow);

    // Cleanup and exit
    SDLUtils::cleanup(window, renderer);
    return 0;
}
