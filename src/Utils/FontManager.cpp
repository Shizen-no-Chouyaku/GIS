// src/Utils/FontManager.cpp

#include "FontManager.h"
#include "../Utils/Utils.h"

bool FontManager::loadFont(const std::string& path, int size) {
    std::lock_guard<std::mutex> lock(fontMutex);
    if (font) {
        font.reset(); // Close existing font
    }
    TTF_Font* newFont = TTF_OpenFont(path.c_str(), size);
    if (!newFont) {
        Utils::logError("Failed to load font: " + path + " Error: " + TTF_GetError());
        return false;
    }
    font = std::shared_ptr<TTF_Font>(newFont, TTF_CloseFont);
    return true;
}

TTF_Font* FontManager::getFont() const {
    std::lock_guard<std::mutex> lock(fontMutex);
    return font.get();
}
