// src/UI/Windows/SettingsWindow.cpp

#include "SettingsWindow.h"
#include "../../Components/Dropdown.h"
#include "../../../Utils/Utils.h"
#include <SDL2/SDL_syswm.h>     // For platform checks (file dialogs)
#include <iostream>
#include <cstdio>    // popen, pclose
#include <memory>    // shared_ptr
#include <algorithm>
#include <fstream>   // for reading file dialog results

static const int WINDOW_WIDTH_DEFAULT = 600;
static const int WINDOW_HEIGHT_DEFAULT = 400;
static const int BORDER_THICKNESS = 0;
static const int TITLEBAR_HEIGHT = 30;  // Draggable region
static const int TAB_BUTTON_HEIGHT = 25;
static const int TAB_BUTTON_WIDTH = 110;

SettingsWindow::SettingsWindow(SDL_Renderer* renderer, SDL_Window* window)
    : renderer(renderer),
      window(window),
      needsRedrawFlag(false),
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
    for (size_t i = 0; i < availableFonts.size(); i++) {
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
    {
        int bestDistance = 999999;
        for (size_t i = 0; i < availableResolutions.size(); i++) {
            int dw = availableResolutions[i].first - config.resolutionWidth;
            int dh = availableResolutions[i].second - config.resolutionHeight;
            int dist = dw * dw + dh * dh;
            if (dist < bestDistance) {
                bestDistance = dist;
                currentResIndex = static_cast<int>(i);
            }
        }
    }

    // Create the UI (tab buttons, save/close)
    initUI();
    createTitleTexture();

    // Instantiate the GeneralTab, passing references it needs
    generalTab = std::make_shared<GeneralTab>(
        renderer,
        font,
        window,
        config,
        availableFonts,
        currentFontIndex,
        availableResolutions,
        currentResIndex,
        [this]() { return this->openFileDialog(); } // Lambda capturing 'this'
    );

    // Instantiate the Layers tab (unchanged)
    layersTab = std::make_shared<LayersTab>();
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

void SettingsWindow::initUI() {
    SDL_Color bgColor    = {200, 200, 200, 255};
    SDL_Color hoverColor = {170, 170, 170, 255};

    // General tab button
    auto generalTabBtn = std::make_shared<Button>(
        renderer,
        font,
        "General",
        0,
        TITLEBAR_HEIGHT,
        TAB_BUTTON_WIDTH,
        TAB_BUTTON_HEIGHT,
        [this]() {
            currentTab = Tab::GENERAL;
            needsRedrawFlag = true;
        },
        bgColor,
        hoverColor
    );
    tabButtons.push_back(generalTabBtn);
    tabButtonPositions.emplace_back(0, TITLEBAR_HEIGHT);

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
            needsRedrawFlag = true;
        },
        bgColor,
        hoverColor
    );
    tabButtons.push_back(layersTabBtn);
    tabButtonPositions.emplace_back(0, TITLEBAR_HEIGHT + TAB_BUTTON_HEIGHT);

    // Create saveBtn (always visible)
    saveBtn = std::make_shared<Button>(
        renderer,
        font,
        "Save",
        static_cast<int>(TAB_BUTTON_WIDTH) + 20,
        rect.h - 40, // near bottom
        80,
        25,
        [this]() {
            // Save to ../config/settings.json
            // The "GeneralTab" already sets config.fontPath / config.resWidth, etc.
            // So we just do the final save here
            ConfigManager::saveConfig(config);
            Utils::logInfo("Settings saved.");
        }
    );

    // Create closeBtn (always visible)
    closeBtn = std::make_shared<Button>(
        renderer,
        font,
        "Close",
        static_cast<int>(TAB_BUTTON_WIDTH) + 110,
        rect.h - 40,
        80,
        25,
        [this]() {
            // Remove ourselves from UI
            if (onClose) {
                onClose();
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

    // 2. Delegate events to tab buttons
    for (auto& tb : tabButtons) {
        if (tb->handleEvent(event)) {
            handled = true;
            needsRedrawFlag = true;
        }
    }

    // 3. Always handle events for Save & Close
    if (saveBtn->handleEvent(event)) {
        handled = true;
        needsRedrawFlag = true;
    }
    if (closeBtn->handleEvent(event)) {
        handled = true;
        needsRedrawFlag = true;
    }

    // 4. Delegate events to the current tab
    if (currentTab == Tab::GENERAL) {
        if (generalTab->handleEvent(event)) {
            handled = true;
            needsRedrawFlag = true;
        }
    }
    else if (currentTab == Tab::LAYERS) {
        if (layersTab->handleEvent(event)) {
            handled = true;
            needsRedrawFlag = true;
        }
    }

    return true; // We consume the event
}

void SettingsWindow::update() {
    if (!visible) return;

    // Update tab buttons
    for (auto& btn : tabButtons) {
        btn->update();
    }

    // Update Save & Close buttons
    saveBtn->update();
    closeBtn->update();

    // Update the active tab
    if (currentTab == Tab::GENERAL) {
        generalTab->update();
    }
    else if (currentTab == Tab::LAYERS) {
        layersTab->update();
    }
}

void SettingsWindow::render(SDL_Renderer* renderer) {
    if (!visible) return;

    // Clipping to our rect
    SDL_RenderSetClipRect(renderer, &rect);

    // Draw background
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderFillRect(renderer, &rect);

    // Draw border (currently thickness = 0, so nothing visible)
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
    for (auto& tb : tabButtons) {
        tb->render(renderer);
    }

    // Render whichever tab is active
    if (currentTab == Tab::GENERAL) {
        generalTab->render(renderer, rect, TAB_BUTTON_WIDTH, TITLEBAR_HEIGHT);
    }
    else if (currentTab == Tab::LAYERS) {
        layersTab->render(renderer, font, rect, TAB_BUTTON_WIDTH, TITLEBAR_HEIGHT);
    }

    // Always render the Save & Close buttons
    saveBtn->render(renderer);
    closeBtn->render(renderer);

    SDL_RenderSetClipRect(renderer, nullptr);
    needsRedrawFlag = false;
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

    // Update Save & Close positions
    int generalUIX = rect.x + TAB_BUTTON_WIDTH + 20;
    saveBtn->setPosition(generalUIX, rect.y + rect.h - 40);
    closeBtn->setPosition(generalUIX + 90, rect.y + rect.h - 40);

    // Let the General tab reposition its elements
    generalTab->setPosition(rect.x, rect.y, TAB_BUTTON_WIDTH, TITLEBAR_HEIGHT);

    needsRedrawFlag = true; // Flag the need for redraw
}

void SettingsWindow::setSize(int width, int height) {
    rect.w = width;
    rect.h = height;
    needsRedrawFlag = true;

    // Re-position the Save & Close at the new bottom
    int generalUIX = rect.x + TAB_BUTTON_WIDTH + 20;
    saveBtn->setPosition(generalUIX, rect.y + rect.h - 40);
    closeBtn->setPosition(generalUIX + 90, rect.y + rect.h - 40);

    // Let the General tab reposition its elements
    generalTab->setPosition(rect.x, rect.y, TAB_BUTTON_WIDTH, TITLEBAR_HEIGHT);
}

void SettingsWindow::onWindowResize(int newWidth, int newHeight) {
    // Clamp the window within the new dimensions
    if (rect.x + rect.w > newWidth) {
        rect.x = (newWidth - rect.w) / 2;
    }
    if (rect.y + rect.h > newHeight) {
        rect.y = (newHeight - rect.h) / 2;
    }
    setPosition(rect.x, rect.y);
}

std::string SettingsWindow::openFileDialog() {
#if defined(__linux__)
    FILE* f = popen("zenity --file-selection --file-filter='*.ttf' --title='Select a TTF font' 2>/dev/null", "r");
    if(!f) return "";
    char buffer[1024] = {0};
    if(fgets(buffer, 1024, f) != nullptr) {
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
    char tmpFile[256];
    tmpnam(tmpFile); // create a temp filename to store result

    std::string cmd = "powershell -Command \""
      "$f=New-Object System.Windows.Forms.OpenFileDialog;"
      "$f.Filter='TrueType fonts|*.ttf';"
      "If($f.ShowDialog() -eq 'OK'){"
         "Out-File -FilePath '" + std::string(tmpFile) + "' -InputObject $f.FileName"
      "}\"";
    system(cmd.c_str());

    // Read back the result
    std::ifstream fin(tmpFile);
    if(!fin.is_open()) return "";
    std::string line;
    std::getline(fin, line);
    fin.close();
    remove(tmpFile);
    return line;
#else
    // On other OS, return empty
    return "";
#endif
}
