// src/Utils/MainLoop.cpp

#include "MainLoop.h"
#include "../Rendering/TileRenderer.h"
#include "../UI/UIManager.h"
#include "../UI/MapWindow.h"
#include "../Utils/Utils.h"
#include <SDL2/SDL.h>

void runMainLoop(SDL_Window* window, SDL_Renderer* renderer, UIManager& uiManager, MapWindow& mapWindow) {
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

            // Pass all events to UIManager
            uiManager.handleEvent(event);
        }

        uiManager.update();

        // Determine if UIManager needs redraw
        bool needsRedraw = uiManager.needsRedraw();

        if (needsRedraw) { // Use combined needsRedraw flag
            // Clear the entire screen (or specific UI areas if needed)
        //    SDL_Log("Clearing screen for redraw");
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
            SDL_RenderClear(renderer);
        }

        // Render all UI components
        uiManager.render();

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}
