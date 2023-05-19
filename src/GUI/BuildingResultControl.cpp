//
// Created by goodm on 14.04.2023.
//

#include "BuildingResultControl.h"
#include "GameWindow.h"
#include "PawnManagerClient.h"
#include <SFML/Graphics.hpp>

BuildingResultControl::BuildingResultControl(GameWindow &win, const sf::IntRect &rect) : Control(win, rect), idea(nullptr) {}
void BuildingResultControl::updateAndRender() {
    Control::updateAndRender();
    sf::FloatRect box = win.getGridRectangle(rect);
    sf::Vector2f center = box.getPosition() + box.getSize() * (1 / 2.0f);
    if(idea) {
        PawnManagerClient::buildingRenderer->drawBuilding(*idea, center);
        sf::Text t(idea->name, PawnManagerClient::fontManager.f);
        t.setCharacterSize(20);
        t.setFillColor(sf::Color::Black);
        sf::Vector2f delta = sf::Vector2f(0, idea->radius * 1.5) + sf::Vector2f(-t.getLocalBounds().getSize().x * 0.5f, 0);
        t.setPosition(center + delta + sf::Vector2f(0, 5));
        PawnManagerClient::window->draw(t);
    }
}
