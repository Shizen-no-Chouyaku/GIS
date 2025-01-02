// src/Rendering/Viewport.h
#ifndef VIEWPORT_H
#define VIEWPORT_H

struct Viewport {
    double centerLat;
    double centerLon;
    int zoom;
    int windowWidth;
    int windowHeight;

    // Define zoom limits
    static constexpr int MIN_ZOOM = 2;
    static constexpr int MAX_ZOOM = 19;
};

#endif // VIEWPORT_H
