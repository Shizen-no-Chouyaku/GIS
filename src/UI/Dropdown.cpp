// src/UI/Dropdown.cpp

#include "Dropdown.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>

Dropdown::Dropdown(SDL_Renderer* renderer,
                   TTF_Font* font,
                   const std::vector<std::string>& items,
                   int x, int y,
                   int width, int height,
                   int selectedIndex,
                   SDL_Color bgColor,
                   SDL_Color hoverColor,
                   std::function<void(int)> onSelect)
    : renderer(renderer),
      font(font),
      items(items),
      selectedIndex(selectedIndex),
      expanded(false),
      needRedraw(true),
      bgColor(bgColor),
      hoverColor(hoverColor),
      textColor({0,0,0,255}),
      onSelect(onSelect),
      hoverItemIndex(-1)
{
    rect = { x, y, width, height };
}

void Dropdown::setFont(TTF_Font* newFont) {
    font = newFont;
    // If Dropdown maintains any font-dependent textures, recreate them here
    // For simplicity, we'll set the needRedraw flag
    needRedraw = true;
}

void Dropdown::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mx = event.button.x;
        int my = event.button.y;
        if (event.button.button == SDL_BUTTON_LEFT) {
            // Check if clicked inside our rect
            if (mx >= rect.x && mx <= rect.x + rect.w &&
                my >= rect.y && my <= rect.y + rect.h) {
                // Toggle expanded
                expanded = !expanded;
                needRedraw = true;
            }
            else if (expanded) {
                // Possibly clicked in the expanded area?
                // The expanded area is rect.h * items.size() below rect
                int expandedHeight = rect.h * static_cast<int>(items.size());
                int exTop = rect.y + rect.h;
                int exBottom = exTop + expandedHeight;
                if (mx >= rect.x && mx <= rect.x + rect.w &&
                    my >= exTop && my <= exBottom) {
                    // Clicked in an item
                    int clickedIndex = getItemAtY(my);
                    if (clickedIndex >= 0 && clickedIndex < static_cast<int>(items.size())) {
                        selectedIndex = clickedIndex;
                        if (onSelect) {
                            onSelect(selectedIndex);
                        }
                    }
                }
                expanded = false;
                needRedraw = true;
            }
        }
    }
    else if (event.type == SDL_MOUSEMOTION && expanded) {
        int mx = event.motion.x;
        int my = event.motion.y;
        int exTop = rect.y + rect.h;
        int exBottom = exTop + rect.h * static_cast<int>(items.size());
        if (mx >= rect.x && mx <= rect.x + rect.w &&
            my >= exTop && my <= exBottom) {
            hoverItemIndex = getItemAtY(my);
        } else {
            hoverItemIndex = -1;
        }
        needRedraw = true;
    }
}

void Dropdown::update() {
    // Not much to do here, but we do it
}

void Dropdown::render(SDL_Renderer* renderer) {
    // Draw the collapsed box
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &rect);

    // Draw the selected text
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(items.size())) {
        drawText(renderer, items[selectedIndex], rect.x + 5, rect.y + 5);
    }

    // Draw a small down arrow on the right
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect arrowRect = { rect.x + rect.w - 20, rect.y + rect.h/4, 10, rect.h/2 };
    SDL_RenderFillRect(renderer, &arrowRect);

    if (expanded) {
        // Draw the expanded list
        int listX = rect.x;
        int listY = rect.y + rect.h;
        int itemH = rect.h; 
        SDL_Rect bg = { listX, listY, rect.w, itemH * static_cast<int>(items.size()) };
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(renderer, &bg);

        // Draw each item
        for (size_t i=0; i<items.size(); i++){
            SDL_Rect itemRect = { listX, listY + static_cast<int>(i)*itemH, rect.w, itemH };
            if (static_cast<int>(i) == hoverItemIndex) {
                // highlight
                SDL_SetRenderDrawColor(renderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
                SDL_RenderFillRect(renderer, &itemRect);
            }
            drawText(renderer, items[i], itemRect.x+5, itemRect.y+5);
        }
    }

    needRedraw = false;
}

void Dropdown::drawText(SDL_Renderer* renderer, const std::string& txt, int x, int y) {
    if (!font) return;
    SDL_Surface* surf = TTF_RenderText_Blended(font, txt.c_str(), textColor);
    if(!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect r = { x, y, surf->w, surf->h };
    SDL_RenderCopy(renderer, tex, nullptr, &r);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

int Dropdown::getItemAtY(int mouseY) const {
    int listY = rect.y + rect.h;
    int itemH = rect.h;
    int relY = mouseY - listY;
    int idx = relY / itemH;
    return idx;
}

void Dropdown::setItems(const std::vector<std::string>& newItems) {
    items = newItems;
    // If our old selectedIndex is out of range, clamp it
    if (selectedIndex >= static_cast<int>(items.size())) {
        selectedIndex = static_cast<int>(items.size()) - 1;
    }
    needRedraw = true;
}

void Dropdown::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;
    needRedraw = true;
}
