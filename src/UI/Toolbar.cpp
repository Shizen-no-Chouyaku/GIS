#include "Toolbar.h"

Toolbar::Toolbar(SDL_Renderer* renderer)
    : renderer(renderer), toolbarTexture(nullptr), needsRedraw(true) {
    // Create an off-screen texture for the toolbar
    toolbarTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_TARGET, 800, 60);
    if (!toolbarTexture) {
        printf("Failed to create toolbar texture: %s\n", SDL_GetError());
    }
}


Toolbar::~Toolbar() {
    if (toolbarTexture) {
        SDL_DestroyTexture(toolbarTexture);
    }
}

void Toolbar::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION) {
        // Example: Set needsRedraw to true if the mouse hovers over a button
        int x = event.motion.x, y = event.motion.y;
        if (y >= 0 && y < 60) { // Toolbar area
            needsRedraw = true;
        }
    }
}

void Toolbar::drawToTexture() {
    SDL_SetRenderTarget(renderer, toolbarTexture); // Set toolbar texture as the render target
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Toolbar background
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Buttons
    SDL_Rect button = {10, 10, 100, 40};
    SDL_RenderFillRect(renderer, &button);

    // Reset to the default render target (the screen)
    SDL_SetRenderTarget(renderer, nullptr);
}


void Toolbar::render() {
    if (needsRedraw) {
        drawToTexture(); // Redraw to the texture only when needed
        needsRedraw = false;
    }

    // Render the pre-drawn texture to the screen
    SDL_Rect dstRect = {0, 0, 800, 60}; // Toolbar area
    SDL_RenderCopy(renderer, toolbarTexture, nullptr, &dstRect);
}


