#include "MainLoop.h"

void runMainLoop(SDL_Window* window, SDL_Renderer* renderer, UIManager& uiManager) {
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
            uiManager.handleEvent(event);
        }

        uiManager.update();

        if (uiManager.needsRedraw()) { // Use UIManager's needsRedraw method
            SDL_Log("Screen cleared for redraw");
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
            SDL_RenderClear(renderer);
        }

        uiManager.render();

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}
