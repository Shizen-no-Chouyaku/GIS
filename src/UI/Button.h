// src/UI/Button.h

#ifndef BUTTON_H
#define BUTTON_H

#include "UIComponent.h"
#include <functional>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Button : public UIComponent {
public:
    // Constructor with font and color parameters
    Button(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, 
           int x, int y, int width, int height, 
           std::function<void()> onClick,
           SDL_Color backgroundColor = {242, 242, 242, 242}, // Default white
           SDL_Color hoverColor = {220, 220, 220, 255} // Default light gray
    );
    
    // Destructor
    ~Button();

    // UIComponent Overrides
    bool handleEvent(const SDL_Event& event) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    bool needsRedraw() const override { return needsRedrawFlag; }

    // Position and Size
    void setPosition(int x, int y) override;
    void setSize(int width, int height) override;

    // Getters for width and height
    int getWidth() const { return rect.w; }
    int getHeight() const { return rect.h; } // Added method

    // Setters for colors
    void setBackgroundColor(SDL_Color color) { backgroundColor = color; needsRedrawFlag = true; }
    void setHoverColor(SDL_Color color) { hoverColor = color; needsRedrawFlag = true; }

    // **New Methods**: Setters for font and label
    void setFont(TTF_Font* newFont); // Already declared
    void setLabel(const std::string& newLabel); // Already declared

private:
    SDL_Renderer* renderer;
    SDL_Rect rect;
    std::string text;
    SDL_Texture* textTexture;
    SDL_Color textColor;
    std::function<void()> onClick;
    bool isHovered;
    bool needsRedrawFlag;
    TTF_Font* font; // Reference to the font
    
    SDL_Color backgroundColor; // Button background color
    SDL_Color hoverColor; // Button hover color

    // Helper Methods
    void createTextTexture();
};

#endif // BUTTON_H
