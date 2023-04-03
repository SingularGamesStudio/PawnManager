//
// Created by goodm on 30.03.2023.
//

#include "CraftBuildingWindow.h"
#include "../IDmanager.h"
#include "PawnManagerClient.h"
#include "SFML/Graphics/Text.hpp"

CraftBuildingWindow::CraftBuildingWindow(int id) : id(id) {
    slotCounts = sf::Vector2i(5, 5);
}

void CraftBuildingWindow::updateAndRender() {
    if(IDmanager::get(id) == nullptr) {
        PawnManagerClient::winManager.popWindow();
        return;
    }
    GameWindow::updateAndRender();
    sf::FloatRect fr = getWindowRectangle();
    sf::Text t("Craft Building", PawnManagerClient::fontManager.f);
    t.setCharacterSize(20);
    t.setFillColor(sf::Color(0, 0, 0));
    t.setPosition(fr.getPosition() + sf::Vector2f(10, 10));
    PawnManagerClient::window->draw(t);
}

