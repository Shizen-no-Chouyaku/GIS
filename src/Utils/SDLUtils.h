#ifndef SDLUTILS_H
#define SDLUTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class SDLUtils {
public:
    static SDL_Window* createWindow(const std::string& title, int width, int height);
    static SDL_Renderer* createRenderer(SDL_Window* window);
    static bool initializeSDL();
    static void cleanup(SDL_Window* window, SDL_Renderer* renderer);
};

#endif // SDLUTILS_H
