//
// Created by goodm on 16.03.2023.
//

#ifndef PAWNMANAGER_GAMEWINDOWMANAGER_H
#define PAWNMANAGER_GAMEWINDOWMANAGER_H
#include <cstddef>
#include <vector>

#include "GameWindow.h"

class GameWindowManager {
private:
    std::vector<GameWindow*> windows;

public:
    GameWindowManager();
    void pushWindow(GameWindow* win);
    void popWindow();
    void swapWindow(GameWindow* win);
    size_t windowCount();
    void updateAndRender();
};


#endif//PAWNMANAGER_GAMEWINDOWMANAGER_H
