#ifndef LAYERS_TAB_H
#define LAYERS_TAB_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Minimal class that handles the "Layers" tab logic
class LayersTab {
public:
    LayersTab();
    ~LayersTab();

    // Return true if the event was consumed
    bool handleEvent(const SDL_Event& event);

    // Perform any per-frame updates
    void update();

    // Render the layers tab content
    void render(SDL_Renderer* renderer,
                TTF_Font* font,
                const SDL_Rect& parentRect,
                int tabButtonWidth,
                int titlebarHeight);
};

#endif // LAYERS_TAB_H
