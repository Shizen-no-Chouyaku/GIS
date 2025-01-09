// src/UI/Components/Dropdown.cpp

#include "Dropdown.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include <sstream>
#include <iostream>

// If you need single-dropdown behavior:
Dropdown* Dropdown::currentlyExpandedDropdown = nullptr;

std::vector<std::string> Dropdown::wrapTextByWidth(const std::string& text, int maxWidth) {
    std::vector<std::string> lines;
    if (!font || text.empty()) {
        lines.push_back(text);
        return lines;
    }

    std::istringstream iss(text);
    std::string word;
    std::string currentLine;

    while (iss >> word) {
        if (currentLine.empty()) {
            currentLine = word;
        } else {
            std::string testLine = currentLine + " " + word;
            int w=0, h=0;
            TTF_SizeText(font, testLine.c_str(), &w, &h);

            if (w > maxWidth) {
                lines.push_back(currentLine);
                currentLine = word;
            } else {
                currentLine = testLine;
            }
        }
    }
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }
    return lines;
}

// Minimal text truncation
static std::string truncateText(const std::string& text, int maxWidth, TTF_Font* font) {
    if (!font) return text;
    int w=0, h=0;
    TTF_SizeText(font, text.c_str(), &w, &h);
    if (w <= maxWidth) return text;

    std::string truncated = text;
    while (truncated.size() > 3) {
        truncated.pop_back();
        std::string temp = truncated + "...";
        TTF_SizeText(font, temp.c_str(), &w, &h);
        if (w <= maxWidth) {
            return temp;
        }
    }
    return "...";
}

// Constructor
Dropdown::Dropdown(SDL_Renderer* renderer,
                   TTF_Font* font,
                   const std::vector<std::string>& items,
                   int x, int y,
                   int width, int height,
                   int selectedIndex,
                   SDL_Color bgColor,
                   SDL_Color hoverColor,
                   std::function<void(int)> onSelect,
                   const std::string& label)
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
      hoverItemIndex(-1),
      scrollOffset(0),
      label(label)
{
    rect = { x, y, width, height };

    maxDropdownWidth = 250;
    maxDropdownHeight = 200;

    actualDropdownWidth = width; // We'll recalc

    recomputeWrappedItems();
}

void Dropdown::setFont(TTF_Font* newFont) {
    font = newFont;
    needRedraw = true;
    recomputeWrappedItems();
}

void Dropdown::setItems(const std::vector<std::string>& newItems) {
    items = newItems;
    if (selectedIndex >= (int)items.size()) {
        selectedIndex = (int)items.size() - 1;
    }
    needRedraw = true;
    recomputeWrappedItems();
}

void Dropdown::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;
    needRedraw = true;
}

// Recompute all lines (wrap each item) and measure widest line
void Dropdown::recomputeWrappedItems() {
    wrappedItems.clear();
    actualDropdownWidth = 0;

    if (!font) {
        // Fallback
        for (auto& s : items) {
            wrappedItems.push_back({s});
        }
        return;
    }

    int lineSkip = TTF_FontLineSkip(font); // recommended line spacing
    int wrapWidth = maxDropdownWidth - 20; // 20 px horizontal padding

    for (auto& s : items) {
        auto lines = wrapTextByWidth(s, wrapWidth);
        wrappedItems.push_back(lines);

        for (auto& ln : lines) {
            int w=0, h=0;
            TTF_SizeText(font, ln.c_str(), &w, &h);
            if (w > actualDropdownWidth) {
                actualDropdownWidth = w;
            }
        }
    }

    actualDropdownWidth += 20; // horizontal padding
    if (actualDropdownWidth > maxDropdownWidth) {
        actualDropdownWidth = maxDropdownWidth;
    }
    // never narrower than collapsed
    if (actualDropdownWidth < rect.w) {
        actualDropdownWidth = rect.w;
    }
}

bool Dropdown::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mx = event.button.x;
        int my = event.button.y;
        if (event.button.button == SDL_BUTTON_LEFT) {
            if (!expanded) {
                // Collapsed -> Expand
                if (mx >= rect.x && mx <= rect.x + rect.w &&
                    my >= rect.y && my <= rect.y + rect.h)
                {
                    // collapse other
                    if (currentlyExpandedDropdown && currentlyExpandedDropdown != this) {
                        currentlyExpandedDropdown->expanded = false;
                        currentlyExpandedDropdown->needRedraw = true;
                    }
                    expanded = true;
                    needRedraw = true;
                    currentlyExpandedDropdown = this;
                    return true;
                }
            } else {
                // Expanded -> check click
                SDL_Rect expandedRect = { rect.x, rect.y + rect.h, actualDropdownWidth, maxDropdownHeight };
                if (mx >= expandedRect.x && mx <= expandedRect.x + expandedRect.w &&
                    my >= expandedRect.y && my <= expandedRect.y + expandedRect.h)
                {
                    int lineSkip = TTF_FontLineSkip(font);
                    const int topPad = 5;
                    int relY = my - (expandedRect.y + topPad) + scrollOffset;
                    int clickedLine = relY / (lineSkip + 5);

                    int currentLine=0;
                    for (size_t i=0; i<wrappedItems.size(); i++){
                        size_t lc = wrappedItems[i].size();
                        if (clickedLine < currentLine + lc) {
                            selectedIndex = (int)i;
                            if (onSelect) onSelect(selectedIndex);
                            expanded = false;
                            needRedraw = true;
                            currentlyExpandedDropdown = nullptr;
                            return true;
                        }
                        currentLine += lc;
                    }
                } else {
                    expanded = false;
                    needRedraw = true;
                    currentlyExpandedDropdown = nullptr;
                    return true;
                }
            }
        }
    }
    else if (event.type == SDL_MOUSEWHEEL && expanded) {
        int lineSkip = TTF_FontLineSkip(font);
        int scrollAmt = (lineSkip + 5) * 3;
        if (event.wheel.y > 0) {
            scrollOffset -= scrollAmt;
            if (scrollOffset < 0) scrollOffset=0;
            needRedraw = true;
            return true;
        } else if (event.wheel.y < 0) {
            // compute total lines
            int totalLines=0; 
            for (auto &v: wrappedItems) totalLines += v.size();
            int totalH = 10 + totalLines*(lineSkip+5);
            if (totalH > maxDropdownHeight) {
                scrollOffset += scrollAmt;
                int maxScroll = totalH - maxDropdownHeight;
                if (scrollOffset > maxScroll) scrollOffset = maxScroll;
                needRedraw = true;
                return true;
            }
        }
    }
    else if (event.type == SDL_MOUSEMOTION && expanded) {
        int mx= event.motion.x;
        int my= event.motion.y;
        SDL_Rect expandedRect= { rect.x, rect.y+rect.h, actualDropdownWidth, maxDropdownHeight };
        if (mx >= expandedRect.x && mx <= expandedRect.x+expandedRect.w &&
            my >= expandedRect.y && my <= expandedRect.y+expandedRect.h)
        {
            int lineSkip = TTF_FontLineSkip(font);
            const int topPad=5;
            int relY= my - (expandedRect.y+topPad) + scrollOffset;
            int hoveredLine= relY/(lineSkip+5);

            int currentLine=0; 
            int newHover=-1;
            for (size_t i=0; i<wrappedItems.size(); i++){
                size_t lc= wrappedItems[i].size();
                if (hoveredLine < currentLine + (int)lc) {
                    newHover=(int)i;
                    break;
                }
                currentLine += lc;
            }
            if (newHover != hoverItemIndex) {
                hoverItemIndex= newHover;
                needRedraw= true;
            }
        } else {
            if (hoverItemIndex != -1) {
                hoverItemIndex= -1;
                needRedraw= true;
            }
        }
    }
    return false;
}

void Dropdown::update() {
    // nothing dynamic
}

void Dropdown::render(SDL_Renderer* renderer) {
    // 1) collapsed
    SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderDrawRect(renderer, &rect);

    // build the collapsed text
    std::string disp = label.empty() ? "" : label + ": ";
    if (selectedIndex >=0 && selectedIndex<(int)items.size()) {
        disp += items[selectedIndex];
    } else {
        disp += "Select...";
    }
    // truncate
    int mw= rect.w-10;
    std::string truncated = truncateText(disp, mw, font);

    // measure 
    int tw=0, th=0;
    TTF_SizeText(font, truncated.c_str(), &tw, &th);
    int textX= rect.x + 5;
    int textY= rect.y + (rect.h - th)/2;
    drawText(renderer, truncated, textX, textY);

    // arrow
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    int arrowX= rect.x + rect.w -15;
    int arrowY= rect.y + rect.h/2 -3;
    SDL_RenderDrawLine(renderer, arrowX, arrowY, arrowX+6, arrowY);
    SDL_RenderDrawLine(renderer, arrowX+6, arrowY, arrowX+3, arrowY+4);

    // 2) expanded
    if (expanded) {
        int lineSkip = TTF_FontLineSkip(font);

        // how tall
        int totalLines=0;
        for (auto &v: wrappedItems) totalLines+= v.size();
        int neededH= 2 + totalLines*(lineSkip+5);
        int finalH= (neededH> maxDropdownHeight)? maxDropdownHeight : neededH;

        SDL_Rect expandedRect= { rect.x, rect.y + rect.h, actualDropdownWidth, finalH };
        // fill
        SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(renderer, &expandedRect);
        // border
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderDrawRect(renderer, &expandedRect);
        // clip
        SDL_RenderSetClipRect(renderer, &expandedRect);

        int startY= rect.y+ rect.h +2 - scrollOffset;
        int itemIndex=0;
        for (size_t i=0; i<wrappedItems.size(); i++) {
            for (auto &ln : wrappedItems[i]) {
                int drawY= startY + itemIndex*(lineSkip+5);
                if (drawY + lineSkip< expandedRect.y || drawY> expandedRect.y+ finalH) {
                    itemIndex++;
                    continue;
                }
                // highlight
                if ((int)i == hoverItemIndex) {
                    SDL_SetRenderDrawColor(renderer, hoverColor.r, hoverColor.g, hoverColor.b, hoverColor.a);
                    SDL_Rect hiRect = { expandedRect.x+1, drawY, actualDropdownWidth-2, (lineSkip+5)-1 };
                    SDL_RenderFillRect(renderer, &hiRect);
                }
                // center text
                int lw=0, lh=0;
                TTF_SizeText(font, ln.c_str(), &lw, &lh);
                
                // **Here's the minimal fix**: we add +2 to push text down
                // so it doesn't stick to the top edge.
                int textYCentered = drawY + (lineSkip - lh)/2 + 3;

                int textX= expandedRect.x + 10; // left padding
                drawText(renderer, ln, textX, textYCentered);

                itemIndex++;
            }
        }

        SDL_RenderSetClipRect(renderer, nullptr);
    }

    needRedraw= false;
}

void Dropdown::drawText(SDL_Renderer* renderer, const std::string& txt, int x, int y) {
    if (!font) return;
    SDL_Surface* surf= TTF_RenderText_Blended(font, txt.c_str(), textColor);
    if (!surf) return;
    SDL_Texture* tex= SDL_CreateTextureFromSurface(renderer, surf);
    if (!tex) {
        SDL_FreeSurface(surf);
        return;
    }
    SDL_Rect r= { x, y, surf->w, surf->h };
    SDL_RenderCopy(renderer, tex, nullptr, &r);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}
