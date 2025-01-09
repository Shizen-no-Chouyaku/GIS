// src/Rendering/MainLoop.cpp

#include "MainLoop.h"
#include "TileRenderer.h"
#include "../UI/UIManager.h"
#include "../UI/Windows/MapWindow.h"
#include "../Utils/Utils.h"
#include <SDL2/SDL.h>

void runMainLoop(SDL_Window* window, SDL_Renderer* renderer, UIManager& uiManager, MapWindow& mapWindow) {
    bool running = true;
    SDL_Event event;

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    while (running) {
        Uint32 frameStart = SDL_GetTicks();

        // Poll events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            uiManager.handleEvent(event);
        }

        // Update map & UI
        mapWindow.update();
        uiManager.update();

        // Combine flags
        bool mapNeeds = mapWindow.needsRedraw();
        bool uiNeeds  = uiManager.needsRedraw();
        bool needsRedraw = mapNeeds || uiNeeds;

        if (needsRedraw) {
            // **Fix B Implementation Start**

            if (uiNeeds) {
                // Notify TileRenderer to redraw the map
                mapWindow.getTileRenderer().setNeedsRedraw(true);
            }

            // **Fix B Implementation End**

            // Clear entire screen
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer);

            // Render map first
            mapWindow.render(renderer);

            // Render UI on top
            uiManager.render();

            SDL_RenderPresent(renderer);

            // Reset the flags
            mapWindow.getTileRenderer().resetRedrawFlag();
            // Optionally you could do a UI manager "resetAllRedrawFlags()" if needed
        }

        // Delay for fixed FPS
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}
