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

void GameWindowManager::onMouseClick(int x, int y, sf::Mouse::Button button) { windows.back()->onMouseClick(x, y, button); }
void GameWindowManager::onCharInput(int ch) {
    windows.back()->onCharInput(ch);
}
void GameWindowManager::onKeyInput(sf::Keyboard::Key key) {
    windows.back()->onKeyInput(key);
}
