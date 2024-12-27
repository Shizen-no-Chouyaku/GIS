#include "InputHandler.h"
#include "../Utils/Utils.h" // Adjust the path if necessary
#include <algorithm>
#include <cmath>

// Define the panning sensitivity
const double PAN_SENSITIVITY = 0.5; // Further reduced for smoother panning
constexpr double MAX_PAN_DELTA_DEGREES = 5.0;

InputHandler::InputHandler(TileRenderer& renderer)
    : tileRenderer(renderer), panning(false), lastMouseX(0), lastMouseY(0),
      accumulatedDeltaX(0.0), accumulatedDeltaY(0.0)
{
    // Constructor implementation (if needed)
}

void InputHandler::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        // Start panning
        panning = true;
        lastMouseX = event.button.x;
        lastMouseY = event.button.y;
    }
    else if (event.type == SDL_MOUSEBUTTONUP) {
        // Stop panning
        panning = false;
    }
    else if (event.type == SDL_MOUSEMOTION && panning) {
        int deltaX = event.motion.xrel;
        int deltaY = event.motion.yrel;

        // Get current zoom level
        int zoom = tileRenderer.viewport.zoom;

        // Calculate degrees per pixel based on zoom level
        double degreesPerPixel = 360.0 / (256.0 * pow(2.0, zoom));

        // Calculate panning deltas in degrees
        double panningDeltaLon = deltaX * degreesPerPixel;
        double panningDeltaLat = deltaY * degreesPerPixel;

        // Clamp the panning deltas to prevent excessive movement
        panningDeltaLon = std::clamp(panningDeltaLon, -MAX_PAN_DELTA_DEGREES, MAX_PAN_DELTA_DEGREES);
        panningDeltaLat = std::clamp(panningDeltaLat, -MAX_PAN_DELTA_DEGREES, MAX_PAN_DELTA_DEGREES);

        // Accumulate deltas
        accumulatedDeltaX += panningDeltaLon;
        accumulatedDeltaY += panningDeltaLat;
    }

    else if(event.type == SDL_MOUSEWHEEL) {
        Viewport vp = tileRenderer.viewport;
        if(event.wheel.y > 0) { // Scroll up to zoom in
            vp.zoom += 1;
        }
        else if(event.wheel.y < 0) { // Scroll down to zoom out
            vp.zoom -= 1;
        }

        if(vp.zoom < 1) vp.zoom = 1;
        if(vp.zoom > 19) vp.zoom = 19;

        tileRenderer.setViewport(vp);
    }
}

void InputHandler::update() {
    if (accumulatedDeltaX != 0.0 || accumulatedDeltaY != 0.0) {
        Viewport vp = tileRenderer.viewport;
        vp.centerLon -= accumulatedDeltaX; // Adjust longitude
        vp.centerLat += accumulatedDeltaY; // Adjust latitude
        tileRenderer.setViewport(vp);

        // Reset accumulated deltas
        accumulatedDeltaX = 0.0;
        accumulatedDeltaY = 0.0;
    }
}