// src/UI/SettingsWindow.cpp

#include "SettingsWindow.h"
#include "Dropdown.h" // Include Dropdown
#include "../Utils/Utils.h"
#include <SDL2/SDL_syswm.h>     // For platform checks (file dialogs)
#include <iostream>
#include <cstdio>    // popen, pclose
#include <memory>    // shared_ptr
#include <algorithm>
#include <fstream>   // for reading file dialog results

static const int WINDOW_WIDTH_DEFAULT = 600;
static const int WINDOW_HEIGHT_DEFAULT = 400;
static const int BORDER_THICKNESS = 2;
static const int TITLEBAR_HEIGHT = 30;  // Draggable region
static const int TAB_BUTTON_HEIGHT = 25;
static const int TAB_BUTTON_WIDTH = 110;

SettingsWindow::SettingsWindow(SDL_Renderer* renderer, SDL_Window* window)
    : renderer(renderer),
      window(window),
      needsRedrawFlag(false), // Can be retained if used internally
      visible(false),
      currentTab(Tab::GENERAL),
      font(nullptr),
      titleTexture(nullptr),
      draggingWindow(false),
      dragOffsetX(0),
      dragOffsetY(0)
{
    // Load config
    config = ConfigManager::loadConfig(); // from ../config/settings.json

    // Setup rect
    rect.x = 100;
    rect.y = 100;
    rect.w = WINDOW_WIDTH_DEFAULT;
    rect.h = WINDOW_HEIGHT_DEFAULT;

    // Attempt to open our main font
    font = TTF_OpenFont(config.fontPath.c_str(), 16);
    if (!font) {
        std::cerr << "Failed to open font " << config.fontPath
                  << ". Using fallback.\n";
        font = TTF_OpenFont("../resources/fonts/WaukeganLdo-ax19.ttf", 16);
        if (!font) {
            Utils::logError("Failed to load fallback font.");
        }
    }

    // Initialize available fonts
    availableFonts = {
        "../resources/fonts/WaukeganLdo-ax19.ttf",
        "../resources/fonts/SomeOtherFont.ttf",
        "../resources/fonts/YetAnotherFont.ttf"
    };
    // Mark which is currently selected
    currentFontIndex = 0;
    for (size_t i=0; i<availableFonts.size(); i++){
        if (availableFonts[i] == config.fontPath) {
            currentFontIndex = static_cast<int>(i);
            break;
        }
    }

    // Initialize resolutions
    availableResolutions = {
        {1280, 720}, {1600, 900}, {1920, 1080}, {2560, 1440}
    };
    // Mark which is currently selected
    currentResIndex = 0;
    int bestDistance = 999999;
    for (size_t i=0; i<availableResolutions.size(); i++){
        int dw = availableResolutions[i].first - config.resolutionWidth;
        int dh = availableResolutions[i].second - config.resolutionHeight;
        int dist = dw*dw + dh*dh;
        if (dist < bestDistance) {
            bestDistance = dist;
            currentResIndex = static_cast<int>(i);
        }
    }

    initUI();
    createTitleTexture();
}

SettingsWindow::~SettingsWindow() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    if (titleTexture) {
        SDL_DestroyTexture(titleTexture);
        titleTexture = nullptr;
    }
    std::cout << "SettingsWindow destructor.\n";
}

// Remove the conflicting needsRedraw() definition
// bool SettingsWindow::needsRedraw() const {
//     return needsRedrawFlag;
// }

void SettingsWindow::initUI() {
    // We'll have 2 vertical tab buttons: "General" and "Layers"
    SDL_Color bgColor = {200, 200, 200, 255};
    SDL_Color hoverColor = {170, 170, 170, 255};

    // General tab button
    auto generalTabBtn = std::make_shared<Button>(
        renderer,
        font,
        "General",
        0,                 // X will be relative
        TITLEBAR_HEIGHT,   // below the window's top bar
        TAB_BUTTON_WIDTH,
        TAB_BUTTON_HEIGHT,
        [this]() {
            currentTab = Tab::GENERAL;
        },
        bgColor,
        hoverColor
    );
    tabButtons.push_back(generalTabBtn);
    tabButtonPositions.push_back({0, TITLEBAR_HEIGHT});

    // Layers tab button
    auto layersTabBtn = std::make_shared<Button>(
        renderer,
        font,
        "Layers",
        0,
        TITLEBAR_HEIGHT + TAB_BUTTON_HEIGHT,
        TAB_BUTTON_WIDTH,
        TAB_BUTTON_HEIGHT,
        [this]() {
            currentTab = Tab::LAYERS;
        },
        bgColor,
        hoverColor
    );
    tabButtons.push_back(layersTabBtn);
    tabButtonPositions.push_back({0, TITLEBAR_HEIGHT + TAB_BUTTON_HEIGHT});

    // Now the actual UI inside the "General" tab:
    // 1) Font drop-down
    // 2) Install Font button
    // 3) Resolution drop-down
    // 4) Save
    // 5) Close

    SDL_Color ddBg = {220, 220, 220, 255};   // drop-down BG
    SDL_Color ddHover = {200, 200, 200, 255};

    // Create fontDropdown
    fontDropdown = std::make_shared<Dropdown>(
        renderer,
        font,
        availableFonts,
        static_cast<int>(TAB_BUTTON_WIDTH) + 20, // X offset from the left tabs
        TITLEBAR_HEIGHT + 20,       // Y offset from top
        200,                        // width
        25,                         // height
        currentFontIndex,
        ddBg,
        ddHover,
        [this](int selectedIndex) {
            // When user selects a font from the drop-down
            currentFontIndex = selectedIndex;
            // Reload the font immediately
            if (font) {
                TTF_CloseFont(font);
            }
            font = TTF_OpenFont(availableFonts[currentFontIndex].c_str(), 16);
            if (!font) {
                Utils::logError("Failed to open selected font: " + availableFonts[currentFontIndex]);
            }
            // Update all UI components that use the font
            createTitleTexture();
            for (auto& btn : tabButtons) {
                btn->setFont(font);
            }
            for (auto& btn : actionButtons) {
                btn->setFont(font);
            }
            fontDropdown->setFont(font);
            installFontBtn->setFont(font);
            resolutionDropdown->setFont(font);
            saveBtn->setFont(font);
            closeBtn->setFont(font);
        }
    );

    // Create installFontBtn
    installFontBtn = std::make_shared<Button>(
        renderer,
        font,
        "Install Font",
        static_cast<int>(TAB_BUTTON_WIDTH) + 230,
        TITLEBAR_HEIGHT + 20,
        100,
        25,
        [this]() {
            std::string newFontPath = openFileDialog();
            if (!newFontPath.empty()) {
                // Add to availableFonts if not already there
                if (std::find(availableFonts.begin(), availableFonts.end(), newFontPath) == availableFonts.end()) {
                    availableFonts.push_back(newFontPath);
                    // Force the drop-down to refresh
                    fontDropdown->setItems(availableFonts);
                }
            }
        }
    );

    // Create resolutionDropdown
    resolutionDropdown = std::make_shared<Dropdown>(
        renderer,
        font,
        availableResToStrings(availableResolutions),
        static_cast<int>(TAB_BUTTON_WIDTH) + 20,
        TITLEBAR_HEIGHT + 60,
        200,
        25,
        currentResIndex,
        ddBg,
        ddHover,
        [this](int selectedIndex) {
            currentResIndex = selectedIndex;
            // Immediately scale program to this resolution
            auto& sel = availableResolutions[currentResIndex];
            if (window) { 
                SDL_SetWindowSize(window, sel.first, sel.second);
                // UIManager will handle the resize via window events
            }
        }
    );

    // Create saveBtn
    saveBtn = std::make_shared<Button>(
        renderer,
        font,
        "Save",
        static_cast<int>(TAB_BUTTON_WIDTH) + 20,
        rect.h - 40, // near bottom
        80,
        25,
        [this]() {
            // Write to ../config/settings.json
            config.fontPath = availableFonts[currentFontIndex];
            config.resolutionWidth = availableResolutions[currentResIndex].first;
            config.resolutionHeight = availableResolutions[currentResIndex].second;
            ConfigManager::saveConfig(config);
            Utils::logInfo("Settings saved.");
        }
    );

    // Create closeBtn
    closeBtn = std::make_shared<Button>(
        renderer,
        font,
        "Close",
        static_cast<int>(TAB_BUTTON_WIDTH) + 110,
        rect.h - 40,
        80,
        25,
        [this]() {
            // We want to truly remove ourselves from UIManager
            if (onClose) {
                onClose(); // This calls uiManager.removeComponent(this SettingsWindow)
            }
        }
    );
}

void SettingsWindow::createTitleTexture() {
    // Title bar text
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface* surf = TTF_RenderText_Blended(font, "Settings", color);
    if (!surf) {
        std::cerr << "Failed to render SettingsWindow title: " << TTF_GetError() << "\n";
        return;
    }
    titleTexture = SDL_CreateTextureFromSurface(renderer, surf);
    titleRect.x = rect.x + (rect.w - surf->w) / 2;
    titleRect.y = rect.y + 5; 
    titleRect.w = surf->w;
    titleRect.h = surf->h;
    SDL_FreeSurface(surf);
}

bool SettingsWindow::handleEvent(const SDL_Event& event) {
    if (!visible) {
        return false; // Window is not visible; do not handle events
    }

    // Determine if the event is inside the SettingsWindow
    int mx = 0, my = 0;
    if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
        mx = event.button.x;
        my = event.button.y;
    }
    else if (event.type == SDL_MOUSEMOTION) {
        mx = event.motion.x;
        my = event.motion.y;
    }
    else {
        SDL_GetMouseState(&mx, &my);
    }

    bool inSettings = (mx >= rect.x && mx <= rect.x + rect.w &&
                       my >= rect.y && my <= rect.y + rect.h);

    if (!inSettings) {
        return false; // Not within SettingsWindow; do not handle
    }

    bool handled = false;

    // 1. Handle window-specific events like dragging
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (mx >= rect.x && mx <= rect.x + rect.w &&
            my >= rect.y && my <= rect.y + TITLEBAR_HEIGHT &&
            event.button.button == SDL_BUTTON_LEFT) {
            draggingWindow = true;
            dragOffsetX = mx - rect.x;
            dragOffsetY = my - rect.y;
            handled = true;
        }
    }
    else if (event.type == SDL_MOUSEBUTTONUP) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            draggingWindow = false;
            handled = true;
        }
    }
    else if (event.type == SDL_MOUSEMOTION) {
        if (draggingWindow) {
            int newX = event.motion.x - dragOffsetX;
            int newY = event.motion.y - dragOffsetY;
            setPosition(newX, newY);
            handled = true;
        }
    }

    // **Fix: Delegate events to tab buttons first**
    for (auto& tb : tabButtons) {
        if (tb->handleEvent(event)) {
            handled = true;
            needsRedrawFlag = true;
        }
    }

    // 2. Delegate events to child components based on the current tab
    if (currentTab == Tab::GENERAL) {
        // Font Dropdown
        if (fontDropdown->handleEvent(event)) {
            handled = true;
            needsRedrawFlag = true;
        }

        // Install Font Button
        if (installFontBtn->handleEvent(event)) {
            handled = true;
            needsRedrawFlag = true;
        }

        // Resolution Dropdown
        if (resolutionDropdown->handleEvent(event)) {
            handled = true;
            needsRedrawFlag = true;
        }

        // Save Button
        if (saveBtn->handleEvent(event)) {
            handled = true;
            needsRedrawFlag = true;
        }

        // Close Button
        if (closeBtn->handleEvent(event)) {
            handled = true;
            needsRedrawFlag = true;
        }
    }
    else if (currentTab == Tab::LAYERS) {
        // Handle layer-specific events if any
        // Example:
        // if (layerButton->handleEvent(event)) {
        //     handled = true;
        //     needsRedrawFlag = true;
        // }
    }

    // 3. Consume the event to prevent it from reaching underlying components
    return true;
}

void SettingsWindow::update() {
    if (!visible) return;

    // Update tab buttons
    for (auto& btn : tabButtons) {
        btn->update();
    }

    // Update components in the current tab
    if (currentTab == Tab::GENERAL) {
        fontDropdown->update();
        installFontBtn->update();
        resolutionDropdown->update();
        saveBtn->update();
        closeBtn->update();
    }
    else if (currentTab == Tab::LAYERS) {
        // Nothing yet
    }
}

void SettingsWindow::render(SDL_Renderer* renderer) {
    if (!visible) return;

    // Clipping to our rect
    SDL_RenderSetClipRect(renderer, &rect);

    // Draw background
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderFillRect(renderer, &rect);

    // Draw border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for(int i = 0; i < BORDER_THICKNESS; i++) {
        SDL_Rect br = { rect.x + i, rect.y + i, rect.w - 2*i, rect.h - 2*i };
        SDL_RenderDrawRect(renderer, &br);
    }

    // Draw a top bar
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_Rect titleBar = { rect.x, rect.y, rect.w, TITLEBAR_HEIGHT };
    SDL_RenderFillRect(renderer, &titleBar);

    // Render the "Settings" title
    if (titleTexture) {
        SDL_Rect dst = titleRect;
        SDL_RenderCopy(renderer, titleTexture, nullptr, &dst);
    }

    // Render the vertical tab area
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_Rect tabArea = { rect.x, rect.y + TITLEBAR_HEIGHT, TAB_BUTTON_WIDTH, rect.h - TITLEBAR_HEIGHT };
    SDL_RenderFillRect(renderer, &tabArea);

    // Render tab buttons
    for (size_t i = 0; i < tabButtons.size(); i++) {
        auto& btn = tabButtons[i];
        btn->render(renderer);
    }

    // Render contents based on the active tab
    if (currentTab == Tab::GENERAL) {
        fontDropdown->render(renderer);
        installFontBtn->render(renderer);
        resolutionDropdown->render(renderer);
        saveBtn->render(renderer);
        closeBtn->render(renderer);
    }
    else if (currentTab == Tab::LAYERS) {
        // Example placeholder
        SDL_Color color = {0, 0, 0, 255};
        SDL_Surface* surf = TTF_RenderText_Blended(font, "Layers TBD", color);
        if (surf) {
            SDL_Texture* tmp = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_Rect msgRect = { rect.x + TAB_BUTTON_WIDTH + 20, rect.y + TITLEBAR_HEIGHT + 20, surf->w, surf->h };
            SDL_RenderCopy(renderer, tmp, nullptr, &msgRect);
            SDL_DestroyTexture(tmp);
            SDL_FreeSurface(surf);
        }
    }

    SDL_RenderSetClipRect(renderer, nullptr);

    needsRedrawFlag = false; // **Reset the flag after rendering**
}

void SettingsWindow::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;

    // Update titleRect
    if (titleTexture) {
        titleRect.x = rect.x + (rect.w - titleRect.w) / 2;
        titleRect.y = rect.y + 5; 
    }

    // Update tab buttons' positions
    for (size_t i = 0; i < tabButtons.size(); i++) {
        int newX = rect.x + tabButtonPositions[i].first;
        int newY = rect.y + tabButtonPositions[i].second;
        tabButtons[i]->setPosition(newX, newY);
    }

    // Update "General" tab UI positions
    int generalUIX = rect.x + TAB_BUTTON_WIDTH + 20;
    int generalUIY = rect.y + TITLEBAR_HEIGHT + 20;
    fontDropdown->setPosition(generalUIX, generalUIY);
    installFontBtn->setPosition(generalUIX + 210, generalUIY);
    resolutionDropdown->setPosition(generalUIX, generalUIY + 40);
    saveBtn->setPosition(generalUIX, rect.y + rect.h - 40);
    closeBtn->setPosition(generalUIX + 90, rect.y + rect.h - 40);

    needsRedrawFlag = true; // **Flag the need for redraw**
}

void SettingsWindow::setSize(int width, int height) {
    rect.w = width;
    rect.h = height;
    needsRedrawFlag = true; // **Flag the need for redraw**
    // Optionally, adjust child components based on new size
}

void SettingsWindow::onWindowResize(int newWidth, int newHeight) {
    // Possibly clamp our rect so we don't go offscreen
    if(rect.x + rect.w > newWidth){
        rect.x = (newWidth - rect.w)/2;
    }
    if(rect.y + rect.h > newHeight){
        rect.y = (newHeight - rect.h)/2;
    }
    setPosition(rect.x, rect.y);
}

std::vector<std::string> SettingsWindow::availableResToStrings(const std::vector<std::pair<int,int>>& ress) {
    std::vector<std::string> out;
    for (auto& r : ress) {
        out.push_back(std::to_string(r.first) + " x " + std::to_string(r.second));
    }
    return out;
}

// Minimal cross-platform "open file" for TTF. 
// On Linux uses "zenity --file-selection", on Windows uses "powershell [System.Windows.Forms.OpenFileDialog]" trick
std::string SettingsWindow::openFileDialog() {
#if defined(__linux__)
    // We'll popen zenity, read the chosen file from stdout
    FILE* f = popen("zenity --file-selection --file-filter='*.ttf' --title='Select a TTF font' 2>/dev/null", "r");
    if(!f) return "";
    char buffer[1024] = {0};
    if(fgets(buffer, 1024, f) != nullptr) {
        // remove trailing \n if any
        std::string path(buffer);
        if(!path.empty() && path.back() == '\n') {
            path.pop_back();
        }
        pclose(f);
        return path;
    }
    pclose(f);
    return "";
#elif defined(_WIN32)
    // A quick hack using powershell + System.Windows.Forms.OpenFileDialog
    // Real code might use COM or native calls, but let's keep it minimal
    char tmpFile[256];
    tmpnam(tmpFile); // create a temp filename to store result

    std::string cmd = "powershell -Command \""
      "$f=New-Object System.Windows.Forms.OpenFileDialog;"
      "$f.Filter='TrueType fonts|*.ttf';"
      "If($f.ShowDialog() -eq 'OK'){"
         "Out-File -FilePath '" + std::string(tmpFile) + "' -InputObject $f.FileName"
      "}\"";
    system(cmd.c_str());

    // read back the result
    std::ifstream fin(tmpFile);
    if(!fin.is_open()) return "";
    std::string line;
    std::getline(fin, line);
    fin.close();
    remove(tmpFile);
    return line;
#else
    // On other OS, we can't do a system call easily. Return empty => no file chosen
    return "";
#endif
}
