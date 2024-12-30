#include "UIManager.h"

UIManager::UIManager(SDL_Renderer* renderer)
    : renderer(renderer), toolbar(new Toolbar(renderer)) {
        //printf("UIManager initialized.\n");
    }

UIManager::~UIManager() {
    delete toolbar;
}

void UIManager::handleEvent(const SDL_Event& event) {
    toolbar->handleEvent(event);
}

void UIManager::render() {
    //printf("Rendering UIManager! \n");
    toolbar->render();
}
