#include "LayersTab.h"
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_render.h>

LayersTab::LayersTab() {
    // Constructor – nothing special yet
}

LayersTab::~LayersTab() {
    // Destructor – nothing special yet
}

bool LayersTab::handleEvent(const SDL_Event& /*event*/) {
    // Currently no interactive elements in the Layers tab.
    // Return false to indicate "not handled".
    return false;
}

void LayersTab::update() {
    // Nothing to update yet for the Layers tab.
}

void LayersTab::render(SDL_Renderer* renderer,
                       TTF_Font* font,
                       const SDL_Rect& parentRect,
                       int tabButtonWidth,
                       int titlebarHeight)
{
    // Minimal placeholder text that was originally in SettingsWindow
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface* surf = TTF_RenderText_Blended(font, "Layers TBD", color);
    if (surf) {
        SDL_Texture* tmp = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect msgRect = {
            parentRect.x + tabButtonWidth + 20,
            parentRect.y + titlebarHeight + 20,
            surf->w,
            surf->h
        };
        SDL_RenderCopy(renderer, tmp, nullptr, &msgRect);
        SDL_DestroyTexture(tmp);
        SDL_FreeSurface(surf);
    }
}
