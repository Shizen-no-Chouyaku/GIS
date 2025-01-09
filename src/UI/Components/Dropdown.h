// src/UI/Components/Dropdown.h

#ifndef DROPDOWN_H
#define DROPDOWN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <functional>
#include "UIComponent.h"

class Dropdown : public UIComponent {
public:
    // onSelect callback: pass the newly selected index
    Dropdown(SDL_Renderer* renderer,
             TTF_Font* font,
             const std::vector<std::string>& items,
             int x, int y,
             int width, int height,
             int selectedIndex,
             SDL_Color bgColor,
             SDL_Color hoverColor,
             std::function<void(int)> onSelect,
             const std::string& label = ""); // **Added label parameter**

    // Overridden UI methods
    bool handleEvent(const SDL_Event& event) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;

    void setPosition(int x, int y) override;
    void setItems(const std::vector<std::string>& newItems);

    void setFont(TTF_Font* newFont); // **Added setFont**

    bool needsRedraw() const override { return needRedraw; }

    // Additional Methods
    bool isExpanded() const { return expanded; }

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<std::string> items;
    int selectedIndex;
    SDL_Rect rect;
    bool expanded;
    bool needRedraw;

    // Colors
    SDL_Color bgColor;
    SDL_Color hoverColor;
    SDL_Color textColor;

    std::function<void(int)> onSelect;

    // For highlight in the expanded list
    int hoverItemIndex;

    // Text Wrapping and Scrolling
    std::vector<std::vector<std::string>> wrappedItems; // Each item can have multiple lines
    int maxDropdownWidth;
    int maxDropdownHeight;
    int scrollOffset; // In pixels

    // Label for context
    std::string label;

    int actualDropdownWidth;

    // Helper Methods
    void drawText(SDL_Renderer* renderer, const std::string& txt, int x, int y);
    void recomputeWrappedItems();
    std::vector<std::string> wrapTextByWidth(const std::string& text, int maxWidth);

    // Static member to track currently expanded dropdown
    static Dropdown* currentlyExpandedDropdown;
};

#endif // DROPDOWN_H
