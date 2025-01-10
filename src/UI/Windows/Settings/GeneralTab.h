#ifndef GENERAL_TAB_H
#define GENERAL_TAB_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>
#include <vector>
#include <functional>

// Forward declarations
class Dropdown;
class Button;
struct AppConfig;

// This class encapsulates all UI (dropdowns, buttons, etc.)
// specifically for the "General" tab.
class GeneralTab {
public:
    // Constructor
    GeneralTab(SDL_Renderer* renderer,
              TTF_Font* font,
              SDL_Window* window,
              AppConfig& config,
              std::vector<std::string>& availableFonts,
              int& currentFontIndex,
              std::vector<std::pair<int, int>>& availableResolutions,
              int& currentResIndex,
              std::function<std::string()> fileDialogFunc);

    // Destructor
    ~GeneralTab();

    // Handle events specific to the General tab
    bool handleEvent(const SDL_Event& event);

    // Update UI components
    void update();

    // Render the General tab content
    void render(SDL_Renderer* renderer, const SDL_Rect& parentRect,
                int tabButtonWidth, int titlebarHeight);

    // Reposition UI elements when the parent window moves
    void setPosition(int parentX, int parentY, int tabButtonWidth, int titlebarHeight);

private:
    SDL_Renderer* renderer;
    TTF_Font*     font;
    SDL_Window*   window;

    // References to shared data
    AppConfig& config;
    std::vector<std::string>& availableFonts;
    int& currentFontIndex;
    std::vector<std::pair<int, int>>& availableResolutions;
    int& currentResIndex;

    // Function to open file dialog
    std::function<std::string()> openFileDialog;

    // UI components
    std::shared_ptr<Dropdown> fontDropdown;
    std::shared_ptr<Button>   installFontBtn;
    std::shared_ptr<Dropdown> resolutionDropdown;

    // Base positions
    int baseX;
    int baseY;

    // Helper methods
    void reloadSelectedFont();
    void applyResolution(int selectedIndex);
    void installNewFont();
};

#endif // GENERAL_TAB_H
