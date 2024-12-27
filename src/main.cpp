// src/main.cpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Rendering/TileRenderer.h"
#include "UI/InputHandler.h"
#include "Utils/Utils.h"

int main(int argc, char* argv[]) {
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        Utils::logError("SDL_Init Error: " + std::string(SDL_GetError()));
        return 1;
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        Utils::logError("IMG_Init Error: " + std::string(IMG_GetError()));
        SDL_Quit();
        return 1;
    }

    // Create SDL window
    SDL_Window* window = SDL_CreateWindow("Custom GIS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if(!window) {
        Utils::logError("SDL_CreateWindow Error: " + std::string(SDL_GetError()));
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Create SDL renderer with hardware acceleration and VSync
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer) {
        Utils::logError("SDL_CreateRenderer Error: " + std::string(SDL_GetError()));
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Initialize TileRenderer
    TileRenderer tileRenderer(renderer);

    // Initialize InputHandler
    InputHandler inputHandler(tileRenderer);

    bool running = true;
    SDL_Event event;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    while(running) {
        frameStart = SDL_GetTicks();

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
            inputHandler.handleEvent(event);
        }

        // Update InputHandler
        inputHandler.update();

        // Render
        tileRenderer.render();

        // Frame rate control
        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
