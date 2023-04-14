//
// Created by goodm on 13.04.2023.
//

#include "SlotControl.h"
#include "SFML/Graphics/Color.hpp"
#include "GameWindow.h"
#include "PawnManagerClient.h"
#include <numbers>

SlotControl::SlotControl(GameWindow& win, const sf::IntRect& rect) : Control(win, rect), res(Resource::Nothing) {}

void SlotControl::updateAndRender() {
    Control::updateAndRender();
    sf::FloatRect r = win.getGridRectangle(rect);
    sf::Color c;
    c = sf::Color(230, 230, 230);
    win.drawBorderedRect(r, 4, c, sf::Color::Black);
    PawnManagerClient::resourceRenderer->drawResource(res, r.getPosition() + r.getSize() * (1/2.0f),
                                                      std::numbers::pi_v<float> / 4, 15);
}
