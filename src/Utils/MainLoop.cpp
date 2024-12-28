#include "MainLoop.h"

void runMainLoop(SDL_Window* window, SDL_Renderer* renderer, TileRenderer& tileRenderer, InputHandler& inputHandler) {
    bool running = true;
    SDL_Event event;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    while (running) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            inputHandler.handleEvent(event);
        }

        inputHandler.update();
        tileRenderer.updateTiles();
        tileRenderer.render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}
