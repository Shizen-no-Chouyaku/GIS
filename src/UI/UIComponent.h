// UIComponent.h
#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include <SDL.h>

class UIComponent {
public:
    virtual ~UIComponent() {}
    virtual void handleEvent(const SDL_Event& event) = 0;
    virtual void update() = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

    // Optional: Methods for setting position and size
    virtual void setPosition(int x, int y) = 0;
    virtual void setSize(int width, int height) = 0;
};

#endif // UICOMPONENT_H
