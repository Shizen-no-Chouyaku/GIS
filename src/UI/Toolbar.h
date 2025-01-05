// src/UI/Toolbar.h

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "UIComponent.h"
#include "Button.h"           // Include Button class
#include "UIManager.h"       // Include UIManager
#include "SettingsWindow.h"  // **Add this line**
#include <memory>
#include <vector>
#include <SDL2/SDL_ttf.h>

class Toolbar : public UIComponent {
public:
    // Constructor now accepts a reference to UIManager
    Toolbar(SDL_Renderer* renderer, UIManager& uiManager);
    ~Toolbar();

    void handleEvent(const SDL_Event& event) override;
    void update() override;
    void render(SDL_Renderer* renderer) override;

    void setPosition(int x, int y) override;
    void setSize(int width, int height) override;

    void onWindowResize(int newWidth, int newHeight) override;

private:
    SDL_Renderer* renderer;
    SDL_Rect position;
    int nextButtonX;
    std::vector<std::shared_ptr<Button>> buttons;
    TTF_Font* font;

    // **Shared pointer to SettingsWindow**
    std::shared_ptr<SettingsWindow> settingsWindow;

    // Reference to UIManager to add/remove components
    UIManager& uiManager;

    // Method to add a new button dynamically
    void addButton(const std::string& label, std::function<void()> onClick);
};

#endif // TOOLBAR_H
