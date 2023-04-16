//
// Created by goodm on 16.03.2023.
//

#ifndef PAWNMANAGER_PAWNMANAGERCLIENT_H
#define PAWNMANAGER_PAWNMANAGERCLIENT_H

#include "../Player.h"
#include "BuildingRenderer.h"
#include "FontManager.h"
#include "GameWindowManager.h"
#include "PawnRenderer.h"
#include "ResourceRenderer.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window.hpp"
#include "../LocalController.h"

class PawnManagerClient {
public:
    static sf::RenderWindow* window;
    static sf::View view;
    static ResourceRenderer* resourceRenderer;
    static PawnRenderer* pawnRenderer;
    static BuildingRenderer* buildingRenderer;

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
    static void connect(std::string address, int port);

private:
    static void init();
    static void updateAndRender();
    static void buildingRenderDfs(ptr<Building> b, sf::Vector2f center);
    static void onMouseClick(int x, int y, sf::Mouse::Button b);
    static bool onBuildingMouseClick(ptr<Building> b, sf::Vector2f pos, sf::Mouse::Button button);
    static void shutdown();
};


#endif//PAWNMANAGER_PAWNMANAGERCLIENT_H
