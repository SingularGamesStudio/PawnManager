//
// Created by goodm on 16.03.2023.
//

#ifndef PAWNMANAGER_PAWNMANAGERCLIENT_H
#define PAWNMANAGER_PAWNMANAGERCLIENT_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window.hpp"
#include "GameWindowManager.h"
#include "PawnRenderer.h"
#include "../Player.h"
#include "BuildingRenderer.h"
#include "ResourceRenderer.h"
#include "FontManager.h"

class PawnManagerClient {
public:
    static sf::RenderWindow* window;
    static sf::View view;
private:
    static PawnRenderer* pawnRenderer;
    static BuildingRenderer* buildingRenderer;
    static ResourceRenderer* resourceRenderer;
public:
    static ptr<Player> player;
private:
    static double curTime;
    static int selectedBuilding;
public:
    static constexpr float renderScale = 1.5f;
    static GameWindowManager winManager;
    static FontManager fontManager;
    static void run();
    PawnManagerClient() = delete;
    PawnManagerClient(const PawnManagerClient& o) = delete;
    PawnManagerClient& operator=(const PawnManagerClient& other) = delete;
private:
    static void init();
    static void updateAndRender();
    static void buildingRenderDfs(ptr<Building> b, sf::Vector2f center);
    static void onMouseClick(int x, int y, sf::Mouse::Button b);
    static bool onBuildingMouseClick(ptr<Building> b, sf::Vector2f pos, sf::Mouse::Button button);
    static void shutdown();
};


#endif //PAWNMANAGER_PAWNMANAGERCLIENT_H
