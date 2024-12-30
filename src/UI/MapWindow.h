// MapWindow.h
#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include "UIComponent.h"
#include "../Rendering/TileRenderer.h"
#include "InputHandler.h"

class MapWindow : public UIComponent {
public:
    MapWindow(TileRenderer& tileRenderer, InputHandler& inputHandler, SDL_Renderer* renderer);
    ~MapWindow();

    void handleEvent(const SDL_Event& event) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;

    void setPosition(int x, int y) override;
    void setSize(int width, int height) override;

private:
    TileRenderer& tileRenderer;
    InputHandler& inputHandler;
    SDL_Renderer* renderer;
    SDL_Rect mapArea;
};

#endif // MAPWINDOW_H
