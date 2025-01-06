// src/Utils/FontManager.h

#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <SDL2/SDL_ttf.h>
#include <string>
#include <memory>
#include <mutex>

class FontManager {
public:
    static FontManager& getInstance() {
        static FontManager instance;
        return instance;
    }
    
    // Delete copy constructors
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;

    bool loadFont(const std::string& path, int size);
    TTF_Font* getFont() const;

private:
    FontManager() : font(nullptr) {}
    ~FontManager() {}

    std::shared_ptr<TTF_Font> font; // Shared pointer for automatic management
    mutable std::mutex fontMutex;
};

#endif // FONTMANAGER_H
