//
// Created by goodm on 30.03.2023.
//

#include "CraftBuildingWindow.h"
#include "../IDmanager.h"
#include "PawnManagerClient.h"

CraftBuildingWindow::CraftBuildingWindow(PawnManagerClient* client, int id) : GameWindow(client), id(id) {
    slotCounts = sf::Vector2i(5, 5);
}

void CraftBuildingWindow::updateAndRender() {
    if(IDmanager::getBuilding(id) == nullptr) {
        client->winManager.popWindow();
        return;
    }
    GameWindow::updateAndRender();
}

