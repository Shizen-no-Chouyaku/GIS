// MapWindow.h
#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <SDL2/SDL.h>
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

    void setPosition(int x, int y);
    void setSize(int width, int height);

    bool needsRedraw() const override;

    const SDL_Rect& getMapArea() const; // Modified getter to return a const reference

private:
    TileRenderer& tileRenderer;
    InputHandler& inputHandler;
    SDL_Renderer* renderer;
    SDL_Rect mapArea;
};

#endif // MAPWINDOW_H
