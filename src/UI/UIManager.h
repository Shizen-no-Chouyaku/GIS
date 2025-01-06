// src/UI/UIManager.h

#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "UIComponent.h"
#include <vector>
#include <memory>

class UIManager {
public:
    UIManager(SDL_Renderer* renderer);
    ~UIManager();

    void handleEvent(const SDL_Event& event);
    void update();
    void render();

    void addComponent(std::shared_ptr<UIComponent> component);
    void removeComponent(std::shared_ptr<UIComponent> component);

    bool needsRedraw() const;

    void setWindow(SDL_Window* window);
    SDL_Window* getWindow() const { return window; }

private:
    SDL_Renderer* renderer;
    SDL_Window* window;
    std::vector<std::shared_ptr<UIComponent>> components;
    
    // New vector to hold components marked for removal
    std::vector<std::shared_ptr<UIComponent>> toBeRemoved;

    // Helper method to process deferred removals
    void processDeferredRemovals();
};

#endif // UIMANAGER_H
