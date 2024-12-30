#include "MainLoop.h"

void runMainLoop(SDL_Window* window, SDL_Renderer* renderer, MapWindow& mapWindow) {
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
            mapWindow.handleEvent(event); // Delegate event handling to MapWindow
        }

        mapWindow.update();   // Delegate updating to MapWindow
        mapWindow.render();   // Delegate rendering to MapWindow

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}
