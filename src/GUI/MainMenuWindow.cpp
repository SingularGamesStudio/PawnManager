//
// Created by goodm on 16.03.2023.
//

#include "MainMenuWindow.h"

#include "TextInputControl.h"
MainMenuWindow::MainMenuWindow() {
    slotCounts = sf::Vector2i(3, 1);
    controls.push_back(new TextInputControl(*this, sf::IntRect(0, 0, 2, 0)));
}
