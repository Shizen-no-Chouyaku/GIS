// MainLoop.h
#ifndef MAINLOOP_H
#define MAINLOOP_H

#include "../UI/UIManager.h"
#include <SDL.h>

void runMainLoop(SDL_Window* window, SDL_Renderer* renderer, UIManager& uiManager);

#endif // MAINLOOP_H
