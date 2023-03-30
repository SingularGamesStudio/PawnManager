//
// Created by goodm on 16.03.2023.
//

#ifndef PAWNMANAGER_GAMEWINDOW_H
#define PAWNMANAGER_GAMEWINDOW_H

#include "SFML/System/Vector2.hpp"

class PawnManagerClient;

class GameWindow {
protected:
    PawnManagerClient* client;
public:
    explicit GameWindow(PawnManagerClient* client);

    virtual void updateAndRender();
    sf::Vector2i slotCounts;
private:
    void drawBackground();
};


#endif //PAWNMANAGER_GAMEWINDOW_H
