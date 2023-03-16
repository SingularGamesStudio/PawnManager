//
// Created by goodm on 16.03.2023.
//

#ifndef PAWNMANAGER_PAWNMANAGERCLIENT_H
#define PAWNMANAGER_PAWNMANAGERCLIENT_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "GameWindowManager.h"
#include "PawnRenderer.h"

class PawnManagerClient {
private:
    sf::RenderWindow window;
    sf::View view;
    PawnRenderer pawnRenderer;
public:
    GameWindowManager winManager;
    PawnManagerClient();
    void run();
    ~PawnManagerClient();
private:
    void updateAndRender();
};


#endif //PAWNMANAGER_PAWNMANAGERCLIENT_H
