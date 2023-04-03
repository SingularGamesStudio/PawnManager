//
// Created by goodm on 03.04.2023.
//

#ifndef PAWNMANAGER_BUTTONCONTROL_H
#define PAWNMANAGER_BUTTONCONTROL_H

#include "Control.h"
#include <string>
#include <functional>

class ButtonControl : public Control {
public:
    ButtonControl(GameWindow& win, sf::IntRect rect, std::string label, std::function<void()> callback);

    void updateAndRender() override;

    bool mouseOver();

    void onMouseClick(int x, int y, sf::Mouse::Button b) override;

private:
    std::string label;
    std::function<void()> callback;
};


#endif //PAWNMANAGER_BUTTONCONTROL_H
