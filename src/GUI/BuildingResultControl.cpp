//
// Created by goodm on 14.04.2023.
//

#include "BuildingResultControl.h"
#include "GameWindow.h"
#include "PawnManagerClient.h"

BuildingResultControl::BuildingResultControl(GameWindow &win, const sf::IntRect &rect) : Control(win, rect), idea(nullptr) {}
void BuildingResultControl::updateAndRender() {
    Control::updateAndRender();
    sf::FloatRect box = win.getGridRectangle(rect);
    sf::Vector2f center = box.getPosition() + box.getSize() * (1 / 2.0f);
    if(idea) {
        PawnManagerClient::buildingRenderer->drawBuilding(*idea, center);
    }
}
