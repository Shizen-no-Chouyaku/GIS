// src/UI/UIManager.cpp

#include "UIManager.h"
#include <algorithm>

UIManager::UIManager(SDL_Renderer* renderer)
    : renderer(renderer), window(nullptr), internalNeedsRedraw(false) {}

UIManager::~UIManager() {}

void UIManager::setWindow(SDL_Window* window) {
    this->window = window;
}

void UIManager::handleEvent(const SDL_Event& event) {
    // Handle window-resize events first, if any
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {
        if (window) {
            int newWidth, newHeight;
            SDL_GetWindowSize(window, &newWidth, &newHeight);
            // Notify all components
            for (auto& c : components) {
                c->onWindowResize(newWidth, newHeight);
            }
        }
    }

    // Now dispatch input from front-most to back-most by going in reverse
    // so the topmost items see the event first.
    for (auto it = components.rbegin(); it != components.rend(); ++it) {
        bool handled = (*it)->handleEvent(event);
        if (handled) {
            // event was consumed; stop here
            break;
        }
    }

    // Then process any removals
    processDeferredRemovals();
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
    internalNeedsRedraw = true; // Add this line
}

void UIManager::removeComponent(std::shared_ptr<UIComponent> component) {
    // Instead of removing immediately, mark for deferred removal
    toBeRemoved.push_back(component);
    internalNeedsRedraw = true; // Add this line
}

bool UIManager::needsRedraw() const {
    if (internalNeedsRedraw) {
        return true; // Return true if components were added or removed
    }
    // If any UI component needs a redraw, return true
    for (auto& c : components) {
        if (c->needsRedraw()) {
            return true;
        }
    }
    return false;
}

void UIManager::processDeferredRemovals() {
    if (!toBeRemoved.empty()) {
        for (auto& comp : toBeRemoved) {
            components.erase(std::remove(components.begin(), components.end(), comp), components.end());
        }
        toBeRemoved.clear();
    }
}
