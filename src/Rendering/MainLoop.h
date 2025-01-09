#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <SDL2/SDL.h>
#include "../UI/UIManager.h"
#include "../UI/MapWindow.h"

void runMainLoop(SDL_Window* window, SDL_Renderer* renderer, UIManager& uiManager, MapWindow& mapWindow);

#endif // MAINLOOP_H
