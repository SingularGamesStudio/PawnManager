//
// Created by goodm on 03.04.2023.
//

#include "Control.h"

#include "GameWindow.h"
#include "PawnManagerClient.h"

Control::Control(GameWindow& win, sf::IntRect rect) : win(win), rect(rect) {}

void Control::updateAndRender() {}

void Control::onMouseClick(int x, int y, sf::Mouse::Button b) {}

bool Control::mouseOver() {
    sf::FloatRect r = win.getGridRectangle(rect);
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*PawnManagerClient::window));
    return r.contains(mousePos);
}
void Control::onCharInput(int ch) {}
