#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <SDL2/SDL.h>
#include "../UI/MapWindow.h" // Include the MapWindow header

void runMainLoop(SDL_Window* window, SDL_Renderer* renderer, MapWindow& mapWindow);

#endif // MAINLOOP_H
