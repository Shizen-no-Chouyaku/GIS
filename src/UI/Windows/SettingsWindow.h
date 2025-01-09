#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include "../Components/UIComponent.h"
#include "../Components/Button.h"
#include "../Components/Dropdown.h" // Include Dropdown
#include "../../Config/ConfigManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <memory>
#include <functional> // Include for std::function

class SettingsWindow : public UIComponent {
public:
    SettingsWindow(SDL_Renderer* renderer, SDL_Window* window);
    ~SettingsWindow();

    bool handleEvent(const SDL_Event& event) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;

    void setPosition(int x, int y) override;
    void setSize(int width, int height) override;

    void onWindowResize(int newWidth, int newHeight) override;

    bool needsRedraw() const override { return needsRedrawFlag; } // **Always returns false**

    bool isVisible() const { return visible; }
    void setVisible(bool v) { visible = v; }

    // **New Method**: Set the callback to be called when the window is closed
    void setOnCloseCallback(std::function<void()> callback) { onClose = callback; }

private:
    SDL_Renderer* renderer;
    SDL_Window* window; // Pointer to SDL_Window
    SDL_Rect rect;             // Position and size of this window
    bool needsRedrawFlag;     // **Can be removed if not used internally**
    bool visible;

    // The currently loaded config from disk
    AppConfig config;

    // Font used to render text inside the settings window
    TTF_Font* font;

    // Buttons for tabs and actions
    std::vector<std::shared_ptr<Button>> tabButtons;
    std::vector<std::shared_ptr<Button>> actionButtons;

    // Which tab are we on?
    enum class Tab { GENERAL, LAYERS };
    Tab currentTab;

    // For demonstration, we’ll store some alternative fonts
    std::vector<std::string> availableFonts;
    int currentFontIndex;

    // Some resolution options
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

    // **New Member Variables**
    std::shared_ptr<Dropdown> fontDropdown;
    std::shared_ptr<Button> installFontBtn;
    std::shared_ptr<Dropdown> resolutionDropdown;
    std::shared_ptr<Button> saveBtn;
    std::shared_ptr<Button> closeBtn;

    bool draggingWindow;
    int dragOffsetX;
    int dragOffsetY;

    // Helper methods
    std::vector<std::string> availableResToStrings(const std::vector<std::pair<int,int>>& ress);
    std::string openFileDialog();
};

#endif // SETTINGS_WINDOW_H
