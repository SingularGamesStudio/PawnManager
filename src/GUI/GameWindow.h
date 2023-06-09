//
// Created by goodm on 16.03.2023.
//

#ifndef PAWNMANAGER_GAMEWINDOW_H
#define PAWNMANAGER_GAMEWINDOW_H

#include <vector>

#include "Control.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window.hpp"

class PawnManagerClient;

class GameWindow {
public:
    constexpr static float gridCellSize = 64;
    constexpr static float gridCellPadding = 8;
    constexpr static float windowSizeExtension = 16;
    Control* selectedControl;
    GameWindow();
    ~GameWindow();

    virtual void updateAndRender();
    sf::Vector2i slotCounts;

    sf::Vector2f getMinGridCellPos();
    sf::FloatRect getGridCellRectangle(sf::Vector2i pos);
    sf::FloatRect getWindowRectangle();
    sf::FloatRect getGridRectangle(sf::IntRect rect);

    void drawRect(sf::FloatRect rect, sf::Color c);
    void drawBorderedRect(sf::FloatRect rect, float borderSize, sf::Color c, sf::Color border);
    void onMouseClick(int x, int y, sf::Mouse::Button b);
    void onCharInput(int ch);
    void onKeyInput(sf::Keyboard::Key k);

protected:
    std::vector<Control*> controls;

private:
    void drawBackground();
    void drawWindow();
    void updateAndRenderControls();
};


#endif//PAWNMANAGER_GAMEWINDOW_H
