// src/UI/Toolbar.cpp
#include "Toolbar.h"

Toolbar::Toolbar(SDL_Renderer* renderer)
    : renderer(renderer) {
    // Initialize position and size
    position = {0, 0, 800, 60}; // Initial values; will be updated on window resize
}

Toolbar::~Toolbar() {}

void Toolbar::handleEvent(const SDL_Event& event) {
    // Handle toolbar-specific events
}

void Toolbar::update() {
    // Update toolbar state if necessary
}

void Toolbar::render(SDL_Renderer* renderer) {
    // Render the toolbar background
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Light gray
    SDL_RenderFillRect(renderer, &position);
    // Render toolbar buttons/icons here
}

void Toolbar::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
}

void Toolbar::setSize(int width, int height) {
    position.w = width;
    position.h = height;
}

void Toolbar::onWindowResize(int newWidth, int newHeight) {
    // Assuming the toolbar height is 60 pixels
    int toolbarHeight = 60;
    setSize(newWidth, toolbarHeight);
    setPosition(0, 0);
}
