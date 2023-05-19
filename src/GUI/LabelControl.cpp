//
// Created by goodm on 17.05.2023.
//

#include "LabelControl.h"
#include "GameWindow.h"
#include <SFML/Graphics.hpp>
#include "PawnManagerClient.h"

LabelControl::LabelControl(GameWindow &win, sf::IntRect rect, std::string label) : Control(win, rect), label(label) {}
void LabelControl::updateAndRender() {
    Control::updateAndRender();
    sf::FloatRect r = win.getGridRectangle(rect);
    sf::Text t(label, PawnManagerClient::fontManager.f);
    t.setCharacterSize(20);
    t.setFillColor(sf::Color::Black);
    sf::Vector2f delta = (r.getSize() - t.getLocalBounds().getSize()) * 0.5f;
    t.setPosition(r.getPosition() + delta - sf::Vector2f(0, 5));
    PawnManagerClient::window->draw(t);
}
