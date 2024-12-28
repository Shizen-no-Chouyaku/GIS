#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include "../Rendering/TileRenderer.h"
#include "../UI/InputHandler.h"

class MapWindow {
public:
    MapWindow(TileRenderer& tileRenderer, InputHandler& inputHandler);
    ~MapWindow();

    void handleEvent(const SDL_Event& event);
    void update();
    void render();

private:
    TileRenderer& tileRenderer;
    InputHandler& inputHandler;
};

#endif // MAPWINDOW_H
