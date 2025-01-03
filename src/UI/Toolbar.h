// src/UI/Toolbar.h

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "UIComponent.h"
#include "Button.h" // Include the Button class
#include <memory>
#include <vector> // Include for managing multiple buttons

class Toolbar : public UIComponent {
public:
    Toolbar(SDL_Renderer* renderer);
    ~Toolbar();

    void handleEvent(const SDL_Event& event) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;

    void setPosition(int x, int y) override;
    void setSize(int width, int height) override;

    void onWindowResize(int newWidth, int newHeight) override;

    // Method to add a new button dynamically
    void addButton(const std::string& label, std::function<void()> onClick);

private:
    SDL_Renderer* renderer;
    SDL_Rect position;

    int nextButtonX; // Tracks the X position for the next button

    std::vector<std::shared_ptr<Button>> buttons; // Holds all buttons in the toolbar

    TTF_Font* font; // Font used for all buttons
};

#endif // TOOLBAR_H
