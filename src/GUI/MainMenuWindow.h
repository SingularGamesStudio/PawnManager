//
// Created by goodm on 16.03.2023.
//

#ifndef PAWNMANAGER_MAINMENUWINDOW_H
#define PAWNMANAGER_MAINMENUWINDOW_H

#include "GameWindow.h"
#include "TextInputControl.h"

class MainMenuWindow : public GameWindow {
private:
    TextInputControl* textInput;
public:
    MainMenuWindow();
    void updateAndRender() override;
};


#endif//PAWNMANAGER_MAINMENUWINDOW_H
