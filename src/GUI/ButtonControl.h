//
// Created by goodm on 03.04.2023.
//

#ifndef PAWNMANAGER_BUTTONCONTROL_H
#define PAWNMANAGER_BUTTONCONTROL_H

#include <functional>
#include <string>

#include "Control.h"

class ButtonControl : public Control {
public:
    ButtonControl(GameWindow& win, sf::IntRect rect, std::string label, std::function<void()> callback);

    void updateAndRender() override;

    void onMouseClick(int x, int y, sf::Mouse::Button b) override;

private:
    std::string label;
    std::function<void()> callback;
};


#endif//PAWNMANAGER_BUTTONCONTROL_H
