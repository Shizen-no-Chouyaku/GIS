// src/UI/InputHandler.cpp
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
        // Longitude scales linearly
        double degreesPerPixelLon = 360.0 / (256.0 * pow(2.0, zoom));
        
        // Latitude does not scale linearly; use Mercator approximation
        // However, for simplicity, we'll use an approximate degrees per pixel
        // A more accurate approach is implemented in TileRenderer::getMaxLatitudeDelta
        double degreesPerPixelLat = 360.0 / (256.0 * pow(2.0, zoom));

        // Calculate panning deltas in degrees
        double panningDeltaLon = deltaX * degreesPerPixelLon;
        double panningDeltaLat = deltaY * degreesPerPixelLat;

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

        // Clamp zoom level using std::clamp
        vp.zoom = std::clamp(vp.zoom, Viewport::MIN_ZOOM, Viewport::MAX_ZOOM);

        if(vp.zoom == Viewport::MIN_ZOOM) {
            Utils::logInfo("Minimum zoom level reached");
        }
        else if(vp.zoom == Viewport::MAX_ZOOM) {
            Utils::logInfo("Maximum zoom level reached");
        }
        else {
            Utils::logInfo("Zoom level set to " + std::to_string(vp.zoom));
        }

        tileRenderer.setViewport(vp);
    }
}

void InputHandler::update() {
    if (accumulatedDeltaX != 0.0 || accumulatedDeltaY != 0.0) {
        Viewport vp = tileRenderer.viewport;
        vp.centerLon -= accumulatedDeltaX; // Adjust longitude
        vp.centerLat += accumulatedDeltaY; // Adjust latitude

        // Calculate the maximum allowable latitude delta based on current zoom and window size
        double maxLatitudeDelta = tileRenderer.getMaxLatitudeDelta();

        // Define latitude bounds
        constexpr double MAX_LATITUDE = 85.0511;
        constexpr double MIN_LATITUDE = -85.0511;

        // Calculate potential new centerLat
        double potentialCenterLat = vp.centerLat;

        // Ensure that the viewport's visible area stays within latitude bounds
        if (potentialCenterLat + maxLatitudeDelta > MAX_LATITUDE) {
            potentialCenterLat = MAX_LATITUDE - maxLatitudeDelta;
        }
        if (potentialCenterLat - maxLatitudeDelta < MIN_LATITUDE) {
            potentialCenterLat = MIN_LATITUDE + maxLatitudeDelta;
        }

        // Clamp the centerLat within the adjusted limits
        vp.centerLat = std::clamp(potentialCenterLat, MIN_LATITUDE + maxLatitudeDelta, MAX_LATITUDE - maxLatitudeDelta);

        // Additionally, clamp to ensure no overflows due to floating-point inaccuracies
        vp.centerLat = std::clamp(vp.centerLat, MIN_LATITUDE, MAX_LATITUDE);

        // Update the viewport
        tileRenderer.setViewport(vp);

        // Reset accumulated deltas
        accumulatedDeltaX = 0.0;
        accumulatedDeltaY = 0.0;
    }
}
