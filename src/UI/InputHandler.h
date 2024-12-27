// src/UI/InputHandler.h

#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <SDL2/SDL.h>
#include "../Rendering/TileRenderer.h" // Adjust the path if necessary

class InputHandler {
public:
    InputHandler(TileRenderer& renderer); // Constructor to initialize with TileRenderer
    void handleEvent(const SDL_Event& event);
    void update(); // Declare the update method

private:
    TileRenderer& tileRenderer;
    bool panning; // Declare the panning variable
    int lastMouseX;
    int lastMouseY;

    double accumulatedDeltaX; // Declare accumulatedDeltaX
    double accumulatedDeltaY; // Declare accumulatedDeltaY
};

#endif // INPUTHANDLER_H
