// UIComponent.h
#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include <SDL.h>

class UIComponent {
public:
    virtual ~UIComponent() {}
    virtual bool handleEvent(const SDL_Event& event) = 0;
    virtual void update() = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

    // Optional: Methods for setting position and size
    virtual void setPosition(int x, int y) {}
    virtual void setSize(int width, int height) {}
    virtual bool needsRedraw() const { return false; }

    virtual void onWindowResize(int newWidth, int newHeight) {}

};

#endif // UICOMPONENT_H
