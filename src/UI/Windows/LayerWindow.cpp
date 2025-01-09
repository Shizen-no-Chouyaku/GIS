// src/UI/LayerWindow.cpp

#include "LayerWindow.h"
#include "../../Utils/Utils.h"
#include <iostream> // For debugging

LayerWindow::LayerWindow(SDL_Renderer* renderer)
    : renderer(renderer), nextButtonY(0), font(nullptr), titleTexture(nullptr),
      borderColor({0, 0, 0, 255}), // Black border
      borderThickness(2) // 2 pixels thick
{
    // Initialize toolbar position and size with fixed height
    position = {0, 30, 200, 500}; // Adjusted height for testing

    // Initialize TTF_Font
    font = TTF_OpenFont("../resources/fonts/WaukeganLdo-ax19.ttf", 16); // should make this configurable still
    if (!font) {
        Utils::logError("Failed to load font: " + std::string(TTF_GetError()));
        return;
    }

    // Create title texture
    SDL_Color titleColor = { 0, 0, 0, 255 }; // Black color
    SDL_Surface* titleSurface = TTF_RenderText_Blended(font, "Layers go woo", titleColor);
    if (!titleSurface) {
        Utils::logError("TTF_RenderText_Blended Error (Title): " + std::string(TTF_GetError()));
        std::cerr << "Failed to render text surface for title." << std::endl;
    } else {
        titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        if (!titleTexture) {
            Utils::logError("SDL_CreateTextureFromSurface Error (Title): " + std::string(SDL_GetError()));
            std::cerr << "Failed to create texture from title surface." << std::endl;
        } else {
            titleRect = { 
                position.x + (position.w - titleSurface->w) / 2, // Center horizontally within LayerWindow
                position.y + TITLE_PADDING_TOP, // Padding from top of LayerWindow
                titleSurface->w,
                titleSurface->h
            };

            // Initialize nextButtonY to start below the title
            nextButtonY = titleRect.y + titleRect.h + BUTTON_PADDING_TOP;
        }
        SDL_FreeSurface(titleSurface);
    }

    // Add multiple buttons dynamically without margins between them
    addButton("Add Layers", []() {
        // Placeholder for opening the settings popup
        std::cout << "Add Layers button clicked." << std::endl;
    });

    addButton("Edit Layers", []() {
        // Placeholder for Edit action
        std::cout << "Edit Layers button clicked." << std::endl;
    });

    // Add more buttons as needed
}

LayerWindow::~LayerWindow() {
    for (auto& button : buttons) {
        // Buttons are managed by shared_ptr, no need for manual deletion
    }
    if (font) {
        TTF_CloseFont(font);
    }
    if (titleTexture) {
        SDL_DestroyTexture(titleTexture);
    }
    std::cout << "LayerWindow destructor called." << std::endl;
}

void LayerWindow::addButton(const std::string& label, std::function<void()> onClick) {

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
        textHeight = BUTTON_HEIGHT; // default height
    }

    // Calculate button size based on text size and padding
    int buttonWidth = position.w - 2 * borderThickness; // Adjusted width
    int buttonHeight = BUTTON_HEIGHT; // Fixed height

    // Define button colors
    SDL_Color buttonBackground = {242, 242, 242, 242}; // White background
    SDL_Color buttonHover = {220, 220, 220, 255}; // Light gray on hover

    // Create the button with specified colors
    auto button = std::make_shared<Button>(
        renderer,
        font,                               // Pass the font to Button
        label,
        position.x + borderThickness,       // Adjusted X position
        nextButtonY,                        // Y position for the button
        buttonWidth,                        // Adjusted width
        buttonHeight,                       // Fixed height
        onClick,                            // Callback for button click
        buttonBackground,                   // Background color
        buttonHover                         // Hover color
    );

    // Add the button to the vector
    buttons.push_back(button);

    // Update nextButtonY for the next button (no margin between buttons)
    nextButtonY += buttonHeight;
}


bool LayerWindow::handleEvent(const SDL_Event& event) {
    // Step 1: Check if the event is within the LayerWindow’s bounding box
    int mx = 0, my = 0;
    if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {
        mx = event.button.x;
        my = event.button.y;
    } else if (event.type == SDL_MOUSEMOTION) {
        mx = event.motion.x;
        my = event.motion.y;
    } else {
        // For things like mouse wheel or key presses, decide whether to swallow them.
        // Usually, you might pass them to the window if it's "active" or skip otherwise.
        // For brevity, let's just do the bounding box check for down/up/motion.
    }

    bool inLayerWindow = 
        (mx >= position.x && mx <= position.x + position.w &&
         my >= position.y && my <= position.y + position.h);

    if (!inLayerWindow) {
        return false;  // We do not handle it, so let something else handle it
    }

    // Step 2: If inside, pass the event to the layer window’s children (buttons, etc.)
    for (auto& button : buttons) {
        button->handleEvent(event);
        // Typically, if your Button also returns bool, you could check
        // if (button->handleEvent(event)) { break; } 
        // but in your code, the Button handleEvent is still returning void, so that’s optional.
    }
    return true; // We consumed it
}


void LayerWindow::update() {
    for (const auto& button : buttons) {
        button->update();
    }
}

void LayerWindow::render(SDL_Renderer* renderer) {

    // Render the  background
    SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255); // Light gray
    SDL_RenderFillRect(renderer, &position);

    // Render the border
    SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    for (int i = 0; i < borderThickness; ++i) {
        SDL_Rect borderRect = { position.x + i, position.y + i, position.w - 2 * i, position.h - 2 * i };
        SDL_RenderDrawRect(renderer, &borderRect);
    }

    // Render the title
    if (titleTexture) {
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    } else {
        std::cout << "TitleTexture is null, not rendering title." << std::endl;
    }

    // Render all buttons
    for (const auto& button : buttons) {
        button->render(renderer);
    }
}

void LayerWindow::setPosition(int x, int y) {
    position.x = x;
    position.y = y;

    // Update title position
    if (titleTexture) {
        // Recalculate titleRect based on new LayerWindow position
        titleRect.x = x + (position.w - titleRect.w) / 2;
        titleRect.y = y + TITLE_PADDING_TOP; // Slight padding from top

        // Recalculate nextButtonY to start below the title
        nextButtonY = titleRect.y + titleRect.h + BUTTON_PADDING_TOP;
    }

    // Update button positions based on the new LayerWindow position
    for (const auto& button : buttons) {
        button->setPosition(position.x + borderThickness, nextButtonY); // Offset by borderThickness
        nextButtonY += button->getHeight(); // Use button's actual height
    }
}

void LayerWindow::setSize(int width, int height) {
    position.w = width;
    position.h = height;

    // Update title position based on new size
    if (titleTexture) {
        titleRect.x = position.x + (position.w - titleRect.w) / 2;
        // Height remains the same
    }

    // Update button sizes and positions
    for (const auto& button : buttons) {
        // Recalculate button width to match new toolbar width minus borders
        button->setSize(position.w - 2 * borderThickness, BUTTON_HEIGHT);
    }

    // Update button positions
    nextButtonY = titleRect.y + titleRect.h + BUTTON_PADDING_TOP; // Reset to start below title
    for (const auto& button : buttons) {
        button->setPosition(position.x + borderThickness, nextButtonY); // Offset by borderThickness
        nextButtonY += button->getHeight(); // Use button's actual height
    }
}

void LayerWindow::onWindowResize(int newWidth, int newHeight) {
    setSize(position.w, newHeight - position.y); // Keeps width at 200 and adjusts height
}
