// UIManager.cpp
#include "UIManager.h"

UIManager::UIManager(SDL_Renderer* renderer)
    : renderer(renderer) {}

UIManager::~UIManager() {}

void UIManager::handleEvent(const SDL_Event& event) {
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
