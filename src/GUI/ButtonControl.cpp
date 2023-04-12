//
// Created by goodm on 03.04.2023.
//

#include "ButtonControl.h"

#include <utility>

#include "GameWindow.h"
#include "PawnManagerClient.h"
#include "SFML/Graphics/Text.hpp"

ButtonControl::ButtonControl(GameWindow& win, sf::IntRect rect, std::string label, std::function<void()> callback)
    : Control(win, rect),
      label(std::move(label)),
      callback(std::move(callback)) {}

void ButtonControl::updateAndRender() {
    Control::updateAndRender();
    sf::FloatRect r = win.getGridRectangle(rect);
    sf::Color c;
    if (mouseOver()) {
        c = sf::Color(255 - 20, 255 - 20, 255);
    } else {
        c = sf::Color(255, 255, 255);
    }
    win.drawBorderedRect(r, 4, c, sf::Color::Black);
    sf::Text t(label, PawnManagerClient::fontManager.f);
    t.setCharacterSize(20);
    t.setFillColor(sf::Color::Black);
    sf::Vector2f delta = (r.getSize() - t.getLocalBounds().getSize()) * 0.5f;
    t.setPosition(r.getPosition() + delta - sf::Vector2f(0, 5));
    PawnManagerClient::window->draw(t);
}

bool ButtonControl::mouseOver() {
    sf::FloatRect r = win.getGridRectangle(rect);
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*PawnManagerClient::window));
    return r.contains(mousePos);
}

void ButtonControl::onMouseClick(int x, int y, sf::Mouse::Button b) {
    Control::onMouseClick(x, y, b);
    if (mouseOver() && b == sf::Mouse::Button::Left) { callback(); }
}