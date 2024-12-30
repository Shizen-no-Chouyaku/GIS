// Toolbar.h
#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "UIComponent.h"

class Toolbar : public UIComponent {
public:
    Toolbar(SDL_Renderer* renderer);
    ~Toolbar();

    void handleEvent(const SDL_Event& event) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;

    void setPosition(int x, int y) override;
    void setSize(int width, int height) override;

private:
    SDL_Renderer* renderer;
    SDL_Rect position;
    // Add toolbar-specific members here
};

#endif // TOOLBAR_H
