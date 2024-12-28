#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <SDL2/SDL.h>
#include "../Rendering/TileRenderer.h"
#include "../UI/InputHandler.h"

void runMainLoop(SDL_Window* window, SDL_Renderer* renderer, TileRenderer& tileRenderer, InputHandler& inputHandler);

#endif // MAINLOOP_H
