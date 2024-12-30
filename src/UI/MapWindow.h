#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include "../Rendering/TileRenderer.h"
#include "../UI/InputHandler.h"
#include <SDL2/SDL.h>
#include "UIManager.h" // Include the new UIManager class

class MapWindow {
public:
    MapWindow(TileRenderer& tileRenderer, InputHandler& inputHandler, SDL_Renderer* renderer);
    ~MapWindow();

    void handleEvent(const SDL_Event& event);
    void update();
    void render();

private:
    TileRenderer& tileRenderer;
    InputHandler& inputHandler;
    UIManager uiManager; // Add the UIManager
};

#endif // MAPWINDOW_H
