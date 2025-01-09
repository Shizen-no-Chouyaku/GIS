#ifndef LAYERWINDOW_H
#define LAYERWINDOW_H

#include "../Components/UIComponent.h"
#include "../Components/Button.h" // Include the Button class
#include <memory>
#include <vector> // Include for managing multiple buttons
#include <SDL2/SDL_ttf.h>

class LayerWindow : public UIComponent {
public:
    LayerWindow(SDL_Renderer* renderer);
    ~LayerWindow();

    bool handleEvent(const SDL_Event& event) override;
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

    int nextButtonY; // Tracks the Y position for the next button

    std::vector<std::shared_ptr<Button>> buttons; // Holds all buttons in the toolbar

    TTF_Font* font; // Font used for all buttons

    // Title properties
    SDL_Texture* titleTexture;
    SDL_Rect titleRect;

    // Border properties
    SDL_Color borderColor;
    int borderThickness;

    // Constants for layout
    static const int TITLE_HEIGHT = 40; // Height allocated for the title
    static const int BUTTON_HEIGHT = 30; // Fixed height for buttons
    static const int TITLE_PADDING_TOP = 10; // Padding from the top of LayerWindow to the title
    static const int BUTTON_PADDING_TOP = 10; // Padding between title and first button
};

#endif // LAYERWINDOW_H
