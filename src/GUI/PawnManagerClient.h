//
// Created by goodm on 16.03.2023.
//

#ifndef PAWNMANAGER_PAWNMANAGERCLIENT_H
#define PAWNMANAGER_PAWNMANAGERCLIENT_H

#include "../Core/Player.h"
#include "BuildingRenderer.h"
#include "GameWindowManager.h"
#include "PawnRenderer.h"
#include "Rect.h"
#include "ResourceRenderer.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Vector.h"

class PawnManagerClient {
private:
    sf::RenderWindow window;
    sf::View view;
    PawnRenderer pawnRenderer;
    BuildingRenderer buildingRenderer;
    ResourceRenderer resourceRenderer;
    Player* player;
    double curTime = 0;

public:
    static constexpr float renderScale = 1.5f;
    GameWindowManager winManager;
    PawnManagerClient();
    void run();
    ~PawnManagerClient();

private:
    void updateAndRender();
    void buildingRenderDfs(Building* b, Vec2f center);
};


#endif//PAWNMANAGER_PAWNMANAGERCLIENT_H
