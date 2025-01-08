// src/UI/Dropdown.h

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
             std::function<void(int)> onSelect);

    // Overridden UI methods
    bool handleEvent(const SDL_Event& event) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;

    void setPosition(int x, int y) override;
    void setItems(const std::vector<std::string>& newItems);

    void setFont(TTF_Font* newFont); // **Added setFont**

    bool needsRedraw() const override { return needRedraw; }

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

    void drawText(SDL_Renderer* renderer, const std::string& txt, int x, int y);
    int getItemAtY(int mouseY) const;
};

#endif // DROPDOWN_H
