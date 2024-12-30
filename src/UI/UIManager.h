// UIManager.h
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

private:
    SDL_Renderer* renderer;
    std::vector<std::shared_ptr<UIComponent>> components;
};

#endif // UIMANAGER_H
