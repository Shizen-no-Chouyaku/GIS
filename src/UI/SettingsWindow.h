// src/UI/SettingsWindow.h

#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include "UIComponent.h"
#include "Button.h"
#include "../Config/ConfigManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <memory>
#include <functional> // Include for std::function

class SettingsWindow : public UIComponent {
public:
    SettingsWindow(SDL_Renderer* renderer);
    ~SettingsWindow();

    void handleEvent(const SDL_Event& event) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;

    void setPosition(int x, int y) override;
    void setSize(int width, int height) override;

    void onWindowResize(int newWidth, int newHeight) override;

    bool needsRedraw() const override {
        // this shit is why it keps rendering a blank map!
         return false; }

    bool isVisible() const { return visible; }
    void setVisible(bool v) { visible = v; }

    // **New Method**: Set the callback to be called when the window is closed
    void setOnCloseCallback(std::function<void()> callback) { onClose = callback; }

private:
    SDL_Renderer* renderer;
    SDL_Rect rect;             // Position and size of this window
    bool needsRedrawFlag;
    bool visible;

    // The currently loaded config from disk
    AppConfig config;

    // Font used to render text inside the settings window
    TTF_Font* font;

    // Buttons for tabs and actions
    std::vector<std::shared_ptr<Button>> tabButtons;
    std::vector<std::shared_ptr<Button>> actionButtons;

    // Which tab are we on?
    enum class Tab { FONT, RESOLUTION };
    Tab currentTab;

    // For demonstration, we’ll store some fake alternative fonts
    std::vector<std::string> availableFonts;
    int currentFontIndex;

    // Some fake resolution options
    std::vector<std::pair<int,int>> availableResolutions;
    int currentResIndex;

    // Private methods
    void initUI();
    void createTitleTexture();
    void drawTabContents(SDL_Renderer* renderer);

    // Title text
    SDL_Texture* titleTexture;
    SDL_Rect titleRect;

    // Callback to notify when the window is closed
    std::function<void()> onClose;
    std::vector<std::pair<int, int>> tabButtonPositions;
    std::vector<std::pair<int, int>> actionButtonPositions;
};

#endif // SETTINGS_WINDOW_H
