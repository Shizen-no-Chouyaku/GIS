// src/UI/Toolbar.cpp

#include "Toolbar.h"
#include "../Utils/Utils.h"
#include <iostream> // For debugging

// Define padding around the text inside buttons
const int TEXT_PADDING_X = 10; // Horizontal padding on each side
const int TEXT_PADDING_Y = 5;  // Vertical padding (if needed)

Toolbar::Toolbar(SDL_Renderer* renderer)
    : renderer(renderer), nextButtonX(0), font(nullptr) // Initialize nextButtonX to 0 for no left margin
{
    // Initialize toolbar position and size with fixed height
    position = {0, 0, 1920, 30}; // x, y, width, height

    // Initialize TTF_Font
    font = TTF_OpenFont("../resources/fonts/WaukeganLdo-ax19.ttf", 16);
    if (!font) {
        Utils::logError("TTF_OpenFont Error: " + std::string(TTF_GetError()));
        // Handle error appropriately, e.g., exit or throw exception
        // For now, we'll proceed without buttons if font fails
        return;
    }

    // Add multiple buttons dynamically without margins between them
    addButton("Settings", []() {
        // Placeholder for opening the settings popup
        std::cout << "Settings button clicked." << std::endl;
    });

    addButton("Moo for me", []() {
        // Placeholder for Edit action
        std::cout << "Edit button clicked." << std::endl;
    });

    // Add more buttons as needed
}

Toolbar::~Toolbar() {
    for (auto& button : buttons) {
        // Buttons are managed by shared_ptr, no need for manual deletion
    }
    if (font) {
        TTF_CloseFont(font);
    }
    std::cout << "Toolbar destructor called." << std::endl;
}

void Toolbar::addButton(const std::string& label, std::function<void()> onClick) {

    if (!font) {
        Utils::logError("Cannot add button without a valid font.");
        return;
    }

    // Measure the text size using TTF_SizeText
    int textWidth = 0;
    int textHeight = 0;
    if (TTF_SizeText(font, label.c_str(), &textWidth, &textHeight) != 0) {
        Utils::logError("TTF_SizeText Error: " + std::string(TTF_GetError()));
        // Handle error appropriately, e.g., set default size
        textWidth = 80; // default width
        textHeight = position.h; // default height
    }

    // Calculate button size based on text size and padding
    int buttonWidth = textWidth + 2 * TEXT_PADDING_X;
    int buttonHeight = position.h; // Match toolbar height

    // Create the button
    auto button = std::make_shared<Button>(
        renderer,
        font,                               // Pass the font to Button
        label,
        nextButtonX,                        // Position the button at the current X position
        0,                                  // Y position relative to toolbar
        buttonWidth,                        // Dynamic button width
        buttonHeight,                       // Button height matching toolbar
        onClick                             // Callback for button click
    );

    // Add the button to the vector
    buttons.push_back(button);

    // Update nextButtonX for the next button (no margin between buttons)
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
