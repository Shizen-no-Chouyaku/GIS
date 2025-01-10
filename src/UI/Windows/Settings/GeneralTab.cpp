#include "GeneralTab.h"
#include "../../Components/Dropdown.h"
#include "../../Components/Button.h"
#include "../../../Utils/Utils.h"
#include "../../../Config/ConfigManager.h"  // For AppConfig if needed
#include <algorithm>   // std::find
#include <iostream>    // std::cerr

// Constants matching existing UI offsets/heights
static const int DROPDOWN_WIDTH      = 200;
static const int DROPDOWN_HEIGHT     = 25;
static const int INSTALL_FONT_BTN_W  = 100;
static const int INSTALL_FONT_BTN_H  = 25;

GeneralTab::GeneralTab(SDL_Renderer* renderer,
                       TTF_Font* font,
                       SDL_Window* window,
                       AppConfig& config,
                       std::vector<std::string>& availableFonts,
                       int& currentFontIndex,
                       std::vector<std::pair<int, int>>& availableResolutions,
                       int& currentResIndex,
                       std::function<std::string()> fileDialogFunc)
    : renderer(renderer),
      font(font),
      window(window),
      config(config),
      availableFonts(availableFonts),
      currentFontIndex(currentFontIndex),
      availableResolutions(availableResolutions),
      currentResIndex(currentResIndex),
      openFileDialog(fileDialogFunc),
      baseX(0),
      baseY(0)
{
    // Create fontDropdown
    fontDropdown = std::make_shared<Dropdown>(
        renderer,
        font,
        availableFonts,
        baseX,            // To be set in setPosition()
        baseY,
        DROPDOWN_WIDTH,
        DROPDOWN_HEIGHT,
        currentFontIndex,
        SDL_Color{220, 220, 220, 255}, // ddBg
        SDL_Color{200, 200, 200, 255}, // ddHover
        [this](int selectedIndex) {
            this->currentFontIndex = selectedIndex;
            this->reloadSelectedFont();
        }
    );

    // Create installFontBtn
    installFontBtn = std::make_shared<Button>(
        renderer,
        font,
        "Install Font",
        baseX, // To be set in setPosition()
        baseY,
        INSTALL_FONT_BTN_W,
        INSTALL_FONT_BTN_H,
        [this]() {
            this->installNewFont();
        }
    );

    // Create resolutionDropdown
    // Convert availableResolutions to strings
    std::vector<std::string> resStrings;
    for (const auto& r : availableResolutions) {
        resStrings.emplace_back(std::to_string(r.first) + " x " + std::to_string(r.second));
    }

    resolutionDropdown = std::make_shared<Dropdown>(
        renderer,
        font,
        resStrings,
        baseX,            // To be set in setPosition()
        baseY + 40,       // Positioned below fontDropdown
        DROPDOWN_WIDTH,
        DROPDOWN_HEIGHT,
        currentResIndex,
        SDL_Color{220, 220, 220, 255}, // ddBg
        SDL_Color{200, 200, 200, 255}, // ddHover
        [this](int selectedIndex) {
            this->applyResolution(selectedIndex);
        }
    );
}

GeneralTab::~GeneralTab() {
    // Nothing special to clean up here
}

bool GeneralTab::handleEvent(const SDL_Event& event) {
    bool handled = false;

    // Handle the fontDropdown
    if (fontDropdown->handleEvent(event)) {
        handled = true;
    }

    // Handle installFontBtn
    if (installFontBtn->handleEvent(event)) {
        handled = true;
    }

    // Handle resolutionDropdown
    if (resolutionDropdown->handleEvent(event)) {
        handled = true;
    }

    return handled; // Return true if an event was consumed
}

void GeneralTab::update() {
    // Update the UI elements
    fontDropdown->update();
    installFontBtn->update();
    resolutionDropdown->update();
}

void GeneralTab::render(SDL_Renderer* renderer, const SDL_Rect& parentRect,
                        int tabButtonWidth, int titlebarHeight)
{
    // Render UI elements
    installFontBtn->render(renderer);

    // Decide which dropdown is on top
    bool fontExpanded = fontDropdown->isExpanded();
    bool resExpanded  = resolutionDropdown->isExpanded();

    if (fontExpanded && !resExpanded) {
        // 1) Draw the resolution dropdown in collapsed state
        resolutionDropdown->render(renderer);
        // 2) Then draw the *expanded* font dropdown last
        fontDropdown->render(renderer);
    }
    else if (resExpanded && !fontExpanded) {
        // 1) Draw the font dropdown in collapsed state
        fontDropdown->render(renderer);
        // 2) Then draw the *expanded* resolution dropdown last
        resolutionDropdown->render(renderer);
    }
    else {
        // Both collapsed or both expanded
        fontDropdown->render(renderer);
        resolutionDropdown->render(renderer);
    }
}

void GeneralTab::setPosition(int parentX, int parentY, int tabButtonWidth, int titlebarHeight) {
    // Positioning as per SettingsWindow's layout
    baseX = parentX + tabButtonWidth + 20;
    baseY = parentY + titlebarHeight + 20;

    // Set positions of UI elements
    fontDropdown->setPosition(baseX, baseY);
    installFontBtn->setPosition(baseX + DROPDOWN_WIDTH + 30, baseY);
    resolutionDropdown->setPosition(baseX, baseY + 40);
}

void GeneralTab::reloadSelectedFont() {
    if (font) {
        TTF_CloseFont(font);
    }
    font = TTF_OpenFont(availableFonts[currentFontIndex].c_str(), 16);
    if (!font) {
        Utils::logError("Failed to open selected font: " + availableFonts[currentFontIndex]);
    }
    // Update the font in the dropdowns and the install button
    fontDropdown->setFont(font);
    installFontBtn->setFont(font);
    resolutionDropdown->setFont(font);
}

void GeneralTab::applyResolution(int selectedIndex) {
    currentResIndex = selectedIndex;
    auto& sel = availableResolutions[currentResIndex];
    if (window) {
        SDL_SetWindowSize(window, sel.first, sel.second);
    }
}

void GeneralTab::installNewFont() {
    std::string newFontPath = openFileDialog();
    if (!newFontPath.empty()) {
        // Add to availableFonts if not already there
        if (std::find(availableFonts.begin(), availableFonts.end(), newFontPath) == availableFonts.end()) {
            availableFonts.emplace_back(newFontPath);
            // Refresh the dropdown
            fontDropdown->setItems(availableFonts);
        }
    }
}
