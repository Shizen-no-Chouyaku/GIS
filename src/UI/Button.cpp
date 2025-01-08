// src/UI/Button.cpp

#include "Button.h"
#include "../Utils/Utils.h"
#include <iostream> // For debugging

Button::Button(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, 
               int x, int y, int width, int height, 
               std::function<void()> onClick,
               SDL_Color backgroundColor,
               SDL_Color hoverColor)
    : renderer(renderer), text(text), onClick(onClick), isHovered(false),
      needsRedrawFlag(true), textTexture(nullptr), font(font),
      backgroundColor(backgroundColor), hoverColor(hoverColor)
{
    rect = { x, y, width, height };
    textColor = { 0, 0, 0, 255 }; // Black color with full opacity
    createTextTexture();
}

Button::~Button() {
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
    }
}

bool Button::handleEvent(const SDL_Event& event)  // changed to bool
{
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;
        if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h)
        {
            if (onClick) {
                onClick();
            }
            // We handled this click
            return true; 
        }
    }
    // If we got here, we did not actually consume the event
    return false;
}

void Button::update() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    bool previouslyHovered = isHovered;
    isHovered = (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                 mouseY >= rect.y && mouseY <= rect.y + rect.h);

    if (isHovered != previouslyHovered) {
        needsRedrawFlag = true;
    }
}

void Button::render(SDL_Renderer* renderer) {
    // Render button background
    SDL_Color currentColor = isHovered ? hoverColor : backgroundColor;
    SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    SDL_RenderFillRect(renderer, &rect);

    // Render text
    if (textTexture) {
        int textW, textH;
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        SDL_Rect textRect = {
            rect.x + (rect.w - textW) / 2,
            rect.y + (rect.h - textH) / 2,
            textW,
            textH
        };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        // Optional: Draw underline for hover effect
        if (isHovered) {
            SDL_SetRenderDrawColor(renderer, 70, 130, 180, 255); // Steel blue underline
            SDL_Rect underlineRect = {
                textRect.x, 
                textRect.y + textRect.h + 2, // Position below text
                textRect.w, 
                2 // Thickness of the underline
            };
            SDL_RenderFillRect(renderer, &underlineRect);
        }
    }
}

void Button::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;
    needsRedrawFlag = true;
}

void Button::setSize(int width, int height) {
    rect.w = width;
    rect.h = height;
    needsRedrawFlag = true;
    createTextTexture();
}

void Button::createTextTexture() {
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    if (!font) {
        Utils::logError("Button has no font set.");
        return;
    }

    // Render the text to an SDL_Surface
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    if (!textSurface) {
        Utils::logError("TTF_RenderText_Blended Error: " + std::string(TTF_GetError()));
        return;
    }

    // Create texture from surface
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        Utils::logError("SDL_CreateTextureFromSurface Error: " + std::string(SDL_GetError()));
    }

    SDL_FreeSurface(textSurface);
}

void Button::setFont(TTF_Font* newFont) {
    font = newFont;
    createTextTexture();
    needsRedrawFlag = true;
}

void Button::setLabel(const std::string& newLabel) {
    text = newLabel;
    createTextTexture();
    needsRedrawFlag = true;
}
