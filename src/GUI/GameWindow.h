//
// Created by goodm on 16.03.2023.
//

#ifndef PAWNMANAGER_GAMEWINDOW_H
#define PAWNMANAGER_GAMEWINDOW_H

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/Color.hpp"

class PawnManagerClient;

class GameWindow {
public:
    constexpr static float gridCellSize = 64;
    constexpr static float gridCellPadding = 8;
    constexpr static float windowSizeExtension = 16;
    GameWindow();

    virtual void updateAndRender();
    sf::Vector2i slotCounts;

    sf::Vector2f getMinGridCellPos();
    sf::FloatRect getGridCellRectangle(sf::Vector2i pos);
    sf::FloatRect getWindowRectangle();

    void drawRect(sf::FloatRect rect, sf::Color c);
    void drawBorderedRect(sf::FloatRect rect, float borderSize, sf::Color c, sf::Color border);
private:
    void drawBackground();
    void drawWindow();
};


#endif //PAWNMANAGER_GAMEWINDOW_H
