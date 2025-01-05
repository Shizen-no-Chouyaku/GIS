// src/UI/UIManager.cpp

#include "UIManager.h"

UIManager::UIManager(SDL_Renderer* renderer)
    : renderer(renderer), window(nullptr) {}

UIManager::~UIManager() {}

void UIManager::setWindow(SDL_Window* window) {
    this->window = window;
}

void UIManager::handleEvent(const SDL_Event& event) {
    // Handle window resize events
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        if (window) {
            int newWidth, newHeight;
            SDL_GetWindowSize(window, &newWidth, &newHeight);

            // Notify all components about the window resize
            for (auto& component : components) {
                component->onWindowResize(newWidth, newHeight);
            }
        }
    }

    // Pass the event to all UI components
    for (auto& component : components) {
        component->handleEvent(event);
    }
}

void UIManager::update() {
    for (auto& component : components) {
        component->update();
    }
}

void UIManager::render() {
    for (auto& component : components) {
        component->render(renderer);
    }
}

void UIManager::addComponent(std::shared_ptr<UIComponent> component) {
    components.push_back(component);
}

bool UIManager::needsRedraw() const {
    for (const auto& component : components) {
        if (component->needsRedraw()) { // Assuming UIComponent has a needsRedraw method
            return true;
        }
    }
    return false;
}
