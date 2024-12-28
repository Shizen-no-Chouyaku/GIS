#include "Utils/SDLUtils.h"
#include "Utils/MainLoop.h"
#include "Rendering/TileRenderer.h"
#include "UI/InputHandler.h"

int main(int argc, char* argv[]) {
    // Initialize SDL
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

    // Run the main loop
    runMainLoop(window, renderer, tileRenderer, inputHandler);

    // Cleanup and exit
    SDLUtils::cleanup(window, renderer);
    return 0;
}
