#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SDL2/SDL.h>
#include "Toolbar.h" // Include the Toolbar header

class UIManager {
public:
    UIManager(SDL_Renderer* renderer);
    ~UIManager();

    void handleEvent(const SDL_Event& event);
    void render();

private:
    SDL_Renderer* renderer;
    Toolbar* toolbar;
};

#endif // UIMANAGER_H
