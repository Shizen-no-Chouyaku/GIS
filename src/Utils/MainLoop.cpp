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
            uiManager.handleEvent(event);
            mapWindow.handleEvent(event); // Handle events for MapWindow
        }

        uiManager.update();
        mapWindow.update();

        // Determine if either UIManager or MapWindow needs redraw
        bool needsRedraw = uiManager.needsRedraw() || mapWindow.needsRedraw();

        if (needsRedraw) { // Use combined needsRedraw flag
            // Clear only the map area
            SDL_Log("Clearing map area for redraw");
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background for map area
            SDL_RenderFillRect(renderer, &mapWindow.getMapArea()); // Clear only the map area
        }

        // Render MapWindow first
        mapWindow.render(renderer);

        // Then render UI components
        uiManager.render();

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}
