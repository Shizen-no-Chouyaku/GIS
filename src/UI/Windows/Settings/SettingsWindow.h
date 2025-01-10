#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include "../../Components/UIComponent.h"
#include "../../Components/Button.h"
#include "../../../Config/ConfigManager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <memory>
#include <functional>

// Forward declarations
#include "GeneralTab.h"
#include "LayersTab.h" // Unchanged

// The SettingsWindow is now a container that
// delegates to GeneralTab and LayersTab.
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

    bool needsRedraw() const override { return needsRedrawFlag; }

    bool isVisible() const { return visible; }
    void setVisible(bool v) { visible = v; }

    // Set the callback to be called when the window is closed
    void setOnCloseCallback(std::function<void()> callback) { onClose = callback; }

private:
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Rect rect;
    bool needsRedrawFlag;
    bool visible;

    // The currently loaded config from disk
    AppConfig config;

    // Font used for the title and tabs
    TTF_Font* font;

    // Buttons for tab switching
    std::vector<std::shared_ptr<Button>> tabButtons;

    // Which tab are we on?
    enum class Tab { GENERAL, LAYERS };
    Tab currentTab;

    // For demonstration, store alternative fonts
    std::vector<std::string> availableFonts;
    int currentFontIndex;

    // Resolution options
    std::vector<std::pair<int, int>> availableResolutions;
    int currentResIndex;

    // Title text
    SDL_Texture* titleTexture;
    SDL_Rect titleRect;

    // Callback to notify when the window is closed
    std::function<void()> onClose;
    std::vector<std::pair<int, int>> tabButtonPositions;

    bool draggingWindow;
    int dragOffsetX;
    int dragOffsetY;

    // Always-visible buttons
    std::shared_ptr<Button> saveBtn;
    std::shared_ptr<Button> closeBtn;

    // The tabs:
    std::shared_ptr<GeneralTab> generalTab;
    std::shared_ptr<LayersTab>  layersTab;

    // Private methods
    void initUI();
    void createTitleTexture();

    // The openFileDialog() is still in SettingsWindow so tabs can call it
    std::string openFileDialog();
};

#endif // SETTINGS_WINDOW_H
