//
// Created by goodm on 04.05.2023.
//

#ifndef PAWNMANAGER_TEXTINPUTCONTROL_H
#define PAWNMANAGER_TEXTINPUTCONTROL_H


#include "Control.h"
class TextInputControl : public Control {
public:
    std::string text;
    int pos;
    TextInputControl(GameWindow &win, const sf::IntRect &rect);
    void updateAndRender() override;
    void onCharInput(int ch) override;
    void onKeyInput(sf::Keyboard::Key k) override;
};


#endif//PAWNMANAGER_TEXTINPUTCONTROL_H
