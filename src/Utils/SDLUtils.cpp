// src/Utils/SDLUtils.cpp
#include "SDLUtils.h"
#include "../Utils/Utils.h"
#include <SDL2/SDL_image.h>

bool SDLUtils::initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        Utils::logError("SDL_Init Error: " + std::string(SDL_GetError()));
        return false;
    }

    // Initialize SDL_image with PNG support
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        Utils::logError("IMG_Init failed: " + std::string(IMG_GetError()));
        SDL_Quit();
        return false;
    }

    return true;
}

SDL_Window* SDLUtils::createWindow(const std::string& title, int width, int height) {
    SDL_Window* window = SDL_CreateWindow(title.c_str(),
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          width, height,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        Utils::logError("SDL_CreateWindow Error: " + std::string(SDL_GetError()));
    }
    return window;
}

SDL_Renderer* SDLUtils::createRenderer(SDL_Window* window) {
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        Utils::logError("SDL_CreateRenderer Error: " + std::string(SDL_GetError()));
    }
    return renderer;
}

void SDLUtils::cleanup(SDL_Window* window, SDL_Renderer* renderer) {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    IMG_Quit();
    SDL_Quit();
}
