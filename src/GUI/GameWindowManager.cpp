//
// Created by goodm on 16.03.2023.
//

#include "GameWindowManager.h"

#include <stdexcept>

GameWindowManager::GameWindowManager() {}

void GameWindowManager::pushWindow(GameWindow* win) { windows.push_back(win); }

void GameWindowManager::popWindow() {
    if (windows.empty()) { throw std::overflow_error("Window stack underflow"); }
    delete windows.back();
    windows.pop_back();
}

void GameWindowManager::swapWindow(GameWindow* win) {
    popWindow();
    pushWindow(win);
}

size_t GameWindowManager::windowCount() { return windows.size(); }

void GameWindowManager::updateAndRender() {
    if (!windows.empty()) { windows.back()->updateAndRender(); }
}
