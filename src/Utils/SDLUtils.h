// src/Utils/SDLUtils.h
#ifndef SDLUTILS_H
#define SDLUTILS_H

#include <string>
#include <SDL2/SDL.h>

class SDLUtils {
public:
    static bool initializeSDL();
    static SDL_Window* createWindow(const std::string& title, int width, int height);
    static SDL_Renderer* createRenderer(SDL_Window* window);
    static void cleanup(SDL_Window* window, SDL_Renderer* renderer);
};

#endif // SDLUTILS_H
