// src/UI/Toolbar.cpp

#include "Toolbar.h"
#include "../Utils/Utils.h"
#include "SettingsWindow.h"  // so we can create it
#include <iostream>

const int TEXT_PADDING_X = 10;
const int TEXT_PADDING_Y = 5;

Toolbar::Toolbar(SDL_Renderer* renderer, UIManager& uiManager)
    : renderer(renderer), uiManager(uiManager), nextButtonX(0), font(nullptr)
{
    position = {0, 0, 1920, 30};

    font = TTF_OpenFont("../resources/fonts/WaukeganLdo-ax19.ttf", 16);
    if (!font) {
        Utils::logError("TTF_OpenFont Error: " + std::string(TTF_GetError()));
        // fallback or handle error
        return;
    }

    // ADD BUTTON: SETTINGS
    addButton("Settings", [this]() {
    // Use our member-variable renderer: this->renderer
    auto settingsWindow = std::make_shared<SettingsWindow>(this->renderer);

    // If UIManager has a method getWindow(), use it:
    int winW = 1920;
    int winH = 1080;

    // If you gave UIManager a public getWindow() accessor:
    if (this->uiManager.getWindow()) {
        SDL_GetWindowSize(this->uiManager.getWindow(), &winW, &winH);
    }

    // Center the settings window in the main window:
    settingsWindow->setPosition((winW - 600) / 2, (winH - 400) / 2);
    settingsWindow->setVisible(true);

    // Use this->uiManager to add the component
    this->uiManager.addComponent(settingsWindow);
});


    // ADD BUTTON: MOO FOR ME
    addButton("Moo for me", []() {
        std::cout << "Moo button clicked.\n";
    });
}

Toolbar::~Toolbar() {
    for (auto& button : buttons) {
        // no manual delete needed, shared_ptr
    }
    if (font) {
        TTF_CloseFont(font);
    }
    std::cout << "Toolbar destructor called.\n";
}

void Toolbar::addButton(const std::string& label, std::function<void()> onClick) {
    if (!font) {
        Utils::logError("Cannot add button without a valid font.");
        return;
    }

    int textWidth = 0;
    int textHeight = 0;
    if (TTF_SizeText(font, label.c_str(), &textWidth, &textHeight) != 0) {
        Utils::logError("TTF_SizeText Error: " + std::string(TTF_GetError()));
        textWidth = 80;
        textHeight = position.h;
    }

    int buttonWidth = textWidth + 2 * TEXT_PADDING_X;
    int buttonHeight = position.h;

    auto button = std::make_shared<Button>(
        renderer,
        font,
        label,
        nextButtonX,
        0,
        buttonWidth,
        buttonHeight,
        onClick
    );

    buttons.push_back(button);
    nextButtonX += buttonWidth;
}

void Toolbar::handleEvent(const SDL_Event& event) {
    for (const auto& button : buttons) {
        button->handleEvent(event);
    }
}

void Toolbar::update() {
    for (const auto& button : buttons) {
        button->update();
    }
}

void Toolbar::render(SDL_Renderer* renderer) {
    // Render the toolbar background
    SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255); // Light gray
    SDL_RenderFillRect(renderer, &position);

    // Render all buttons
    for (const auto& button : buttons) {
        button->render(renderer);
    }
}

void Toolbar::setPosition(int x, int y) {
    position.x = x;
    position.y = y;

    // Update button positions based on the new toolbar position
    int currentX = 0; // Start from the left edge of the toolbar
    for (const auto& button : buttons) {
        button->setPosition(currentX, 0); // Y position relative to toolbar
        currentX += button->getWidth(); // Use button's actual width
    }

    // Update nextButtonX in case buttons are repositioned
    nextButtonX = currentX;
}

void Toolbar::setSize(int width, int height) {
    position.w = width;
    position.h = height;

    // Update button sizes based on new toolbar size if needed
    for (const auto& button : buttons) {
        // Recalculate button height to match new toolbar height
        button->setSize(button->getWidth(), height);
    }

    // Update button positions
    int currentX = 0; // Start from the left edge of the toolbar
    for (const auto& button : buttons) {
        button->setPosition(currentX, 0); // Y position relative to toolbar
        currentX += button->getWidth(); // Use button's actual width
    }

    // Update nextButtonX in case buttons are repositioned
    nextButtonX = currentX;
}

void Toolbar::onWindowResize(int newWidth, int newHeight) {
    setSize(newWidth, position.h); // Maintain fixed height
}
