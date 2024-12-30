#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>

class Toolbar {
public:
    Toolbar(SDL_Renderer* renderer);
    ~Toolbar();

    void handleEvent(const SDL_Event& event);
    void render();

private:
    SDL_Renderer* renderer;
    SDL_Texture* toolbarTexture; // Off-screen texture for the toolbar
    bool needsRedraw;            // Track if the toolbar needs to be redrawn

    void drawToTexture();        // Internal function to draw to the texture
};


#endif // TOOLBAR_H
