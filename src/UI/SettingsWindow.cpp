// src/UI/SettingsWindow.cpp

#include "SettingsWindow.h"
#include "../Utils/Utils.h" // if you have a logging utility
#include <iostream>

static const int WINDOW_WIDTH_DEFAULT = 600;
static const int WINDOW_HEIGHT_DEFAULT = 400;
static const int BORDER_THICKNESS = 2;
static const int TITLE_HEIGHT = 30;
static const int TAB_BUTTON_HEIGHT = 25;

SettingsWindow::SettingsWindow(SDL_Renderer* renderer)
    : renderer(renderer),
      needsRedrawFlag(true), // Can be retained if used internally
      visible(false), // start hidden until we show it
      currentTab(Tab::FONT),
      font(nullptr),
      titleTexture(nullptr)
{
    // Load config from disk
    config = ConfigManager::loadConfig();

    // Setup the window rectangle (we can center it later)
    rect.x = 100;
    rect.y = 100;
    rect.w = WINDOW_WIDTH_DEFAULT;
    rect.h = WINDOW_HEIGHT_DEFAULT;

    // Attempt to open a font for drawing text
    // We can either use the config.fontPath or a fallback if TTF_OpenFont fails
    font = TTF_OpenFont(config.fontPath.c_str(), 16);
    if (!font) {
        std::cerr << "SettingsWindow: Failed to open font from config ("
                  << config.fontPath << "): " << TTF_GetError()
                  << "\nUsing fallback.\n";
        font = TTF_OpenFont("../resources/fonts/WaukeganLdo-ax19.ttf", 16);
    }

    // Some fake data for demonstration
    availableFonts = {
        "resources/fonts/WaukeganLdo-ax19.ttf",
        "resources/fonts/SomeOtherFont.ttf",
        "resources/fonts/YetAnotherFont.ttf"
    };
    // Find which index matches our config
    currentFontIndex = 0;
    for (size_t i=0; i<availableFonts.size(); i++){
        if (availableFonts[i] == config.fontPath) {
            currentFontIndex = static_cast<int>(i);
            break;
        }
    }

    availableResolutions = {
        {1280, 720}, {1600, 900}, {1920, 1080}, {2560, 1440}
    };
    // Figure out which resolution is closest to our config
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
}

void SettingsWindow::initUI() {
    // Create the "Font" and "Resolution" tab buttons
    SDL_Color bgColor = {200, 200, 200, 255};
    SDL_Color hoverColor = {170, 170, 170, 255};

    // Font tab
    int fontTabX = BORDER_THICKNESS;
    int fontTabY = BORDER_THICKNESS + TITLE_HEIGHT;
    auto fontTabBtn = std::make_shared<Button>(
        renderer,
        font,
        "Font",
        fontTabX,
        fontTabY,
        80,
        TAB_BUTTON_HEIGHT,
        [this]() {
            currentTab = Tab::FONT;
            // needsRedrawFlag = true; // **Removed to prevent triggering full-screen clear**
        },
        bgColor,
        hoverColor
    );
    tabButtons.push_back(fontTabBtn);
    tabButtonPositions.emplace_back(fontTabX, fontTabY);

    // Resolution tab
    int resTabX = BORDER_THICKNESS + 80;
    int resTabY = BORDER_THICKNESS + TITLE_HEIGHT;
    auto resTabBtn = std::make_shared<Button>(
        renderer,
        font,
        "Resolution",
        resTabX,
        resTabY,
        100,
        TAB_BUTTON_HEIGHT,
        [this]() {
            currentTab = Tab::RESOLUTION;
            // needsRedrawFlag = true; // **Removed**
        },
        bgColor,
        hoverColor
    );
    tabButtons.push_back(resTabBtn);
    tabButtonPositions.emplace_back(resTabX, resTabY);

    // Save button
    int saveBtnX = BORDER_THICKNESS;
    int saveBtnY = rect.h - BORDER_THICKNESS - 40;
    auto saveBtn = std::make_shared<Button>(
        renderer,
        font,
        "Save",
        saveBtnX,
        saveBtnY,
        80,
        30,
        [this]() {
            // Update config from current selections
            config.fontPath = availableFonts[currentFontIndex];
            config.resolutionWidth = availableResolutions[currentResIndex].first;
            config.resolutionHeight = availableResolutions[currentResIndex].second;
            // Save
            ConfigManager::saveConfig(config);
            // needsRedrawFlag = true; // **Removed**
        },
        bgColor,
        hoverColor
    );
    actionButtons.push_back(saveBtn);
    actionButtonPositions.emplace_back(saveBtnX, saveBtnY);

    // Close button
    int closeBtnX = BORDER_THICKNESS + 90;
    int closeBtnY = rect.h - BORDER_THICKNESS - 40;
    auto closeBtn = std::make_shared<Button>(
        renderer,
        font,
        "Close",
        closeBtnX,
        closeBtnY,
        80,
        30,
        [this]() {
            setVisible(false); // hide the settings window
            if (onClose) {
                onClose(); // Notify that the window is closing
            }
            // needsRedrawFlag = true; // **Removed**
        },
        bgColor,
        hoverColor
    );
    actionButtons.push_back(closeBtn);
    actionButtonPositions.emplace_back(closeBtnX, closeBtnY);
}

void SettingsWindow::createTitleTexture() {
    // Create a title, e.g. "Settings"
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface* surf = TTF_RenderText_Blended(font, "Settings", color);
    if (!surf) {
        std::cerr << "Failed to render SettingsWindow title: " << TTF_GetError() << "\n";
        return;
    }
    titleTexture = SDL_CreateTextureFromSurface(renderer, surf);
    if (!titleTexture) {
        std::cerr << "Failed to create SettingsWindow title texture: " << SDL_GetError() << "\n";
        SDL_FreeSurface(surf);
        return;
    }
    titleRect.x = rect.x + (rect.w - surf->w) / 2;
    titleRect.y = rect.y + 2; // small top padding
    titleRect.w = surf->w;
    titleRect.h = surf->h;

    SDL_FreeSurface(surf);
}

void SettingsWindow::handleEvent(const SDL_Event& event) {
    if (!visible) return;

    // Forward events to our tab buttons
    for (auto& btn : tabButtons) {
        btn->handleEvent(event);
    }
    // Forward events to our action buttons
    for (auto& btn : actionButtons) {
        btn->handleEvent(event);
    }
}

void SettingsWindow::update() {
    if (!visible) return;

    // Update our buttons (hover detection, etc.)
    for (auto& btn : tabButtons) {
        btn->update();
    }
    for (auto& btn : actionButtons) {
        btn->update();
    }
}

void SettingsWindow::render(SDL_Renderer* renderer) {
    if (!visible) return;

    // **Always render the SettingsWindow when visible**

    // Set clipping to the SettingsWindow's rect to prevent drawing outside
    SDL_RenderSetClipRect(renderer, &rect);

    // Draw background only within rect
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderFillRect(renderer, &rect);

    // Draw border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i < BORDER_THICKNESS; ++i) {
        SDL_Rect borderRect = {
            rect.x + i, rect.y + i,
            rect.w - (2 * i), rect.h - (2 * i)
        };
        SDL_RenderDrawRect(renderer, &borderRect);
    }

    // Render the title
    if (titleTexture) {
        SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
    }

    // Render tab buttons
    for (auto& btn : tabButtons) {
        btn->render(renderer);
    }

    // Render tab contents
    drawTabContents(renderer);

    // Render action buttons
    for (auto& btn : actionButtons) {
        btn->render(renderer);
    }

    // Remove clipping
    SDL_RenderSetClipRect(renderer, nullptr);

    // **Do not set needsRedrawFlag here**
}


void SettingsWindow::drawTabContents(SDL_Renderer* renderer) {
    // Minimal text rendering to show which tab is active
    // We’ll create a quick surface/texture for demonstration
    std::string infoText;
    if (currentTab == Tab::FONT) {
        infoText = "Current font: " + availableFonts[currentFontIndex];
        infoText += "\n[Use some hypothetical next/previous button? Example only]";
    } else {
        auto& r = availableResolutions[currentResIndex];
        infoText = "Current resolution: " + std::to_string(r.first) + "x" + std::to_string(r.second);
        infoText += "\n[Use some hypothetical next/previous button? Example only]";
    }

    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface* surf = TTF_RenderText_Blended_Wrapped(font, infoText.c_str(), color, rect.w - 20);
    if (!surf) return;
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, surf);

    // Place it somewhere below the tab buttons
    SDL_Rect txtRect;
    txtRect.x = rect.x + 10;
    txtRect.y = rect.y + BORDER_THICKNESS + TITLE_HEIGHT + TAB_BUTTON_HEIGHT + 10;
    txtRect.w = surf->w;
    txtRect.h = surf->h;

    SDL_RenderCopy(renderer, textTexture, nullptr, &txtRect);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(textTexture);
}

void SettingsWindow::setPosition(int x, int y) {
    int dx = x - rect.x;
    int dy = y - rect.y;
    rect.x = x;
    rect.y = y;

    // Move title
    titleRect.x += dx;
    titleRect.y += dy;

    // Reposition tab buttons based on stored relative positions
    for (size_t i = 0; i < tabButtons.size(); ++i) {
        int newX = rect.x + tabButtonPositions[i].first;
        int newY = rect.y + tabButtonPositions[i].second;
        tabButtons[i]->setPosition(newX, newY);
    }

    // Reposition action buttons based on stored relative positions
    for (size_t i = 0; i < actionButtons.size(); ++i) {
        int newX = rect.x + actionButtonPositions[i].first;
        int newY = rect.y + actionButtonPositions[i].second;
        actionButtons[i]->setPosition(newX, newY);
    }

    // **Do not set needsRedrawFlag = true;**
}

void SettingsWindow::setSize(int width, int height) {
    rect.w = width;
    rect.h = height;
    // You’d also want to reposition / resize buttons, text, etc.
    // **Do not set needsRedrawFlag = true;**
}

void SettingsWindow::onWindowResize(int newWidth, int newHeight) {
    // Optionally re-center or do something else. 
    // For example, keep the same size, but ensure it’s not offscreen:
    if (rect.x + rect.w > newWidth) {
        rect.x = (newWidth - rect.w) / 2;
    }
    if (rect.y + rect.h > newHeight) {
        rect.y = (newHeight - rect.h) / 2;
    }
    // Reposition buttons based on the new window position
    setPosition(rect.x, rect.y);
}
