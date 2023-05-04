//
// Created by goodm on 03.04.2023.
//

#ifndef PAWNMANAGER_CONTROL_H
#define PAWNMANAGER_CONTROL_H

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Window/Mouse.hpp"

class GameWindow;

class Control {
public:
    sf::IntRect rect;
    Control(GameWindow& win, sf::IntRect rect);
    virtual void updateAndRender();
    virtual void onMouseClick(int x, int y, sf::Mouse::Button b);
    virtual void onCharInput(int ch);

    bool mouseOver();

protected:
    GameWindow& win;
};


#endif//PAWNMANAGER_CONTROL_H
