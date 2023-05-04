//
// Created by goodm on 13.04.2023.
//

#include "SlotControl.h"
#include "SFML/Graphics/Color.hpp"
#include "GameWindow.h"
#include "PawnManagerClient.h"
#include <numbers>

SlotControl::SlotControl(GameWindow& win, const sf::IntRect& rect) : Control(win, rect), res(Resource::DummyNothing), drawType(0) {}

void SlotControl::updateAndRender() {
    Control::updateAndRender();
    sf::FloatRect r = win.getGridRectangle(rect);
    sf::Color c;
    c = sf::Color(230, 230, 230);
    win.drawBorderedRect(r, 4, c, sf::Color::Black);
    if(drawType == 1) {
        PawnManagerClient::pawnRenderer->drawWorkerPawn(pawnExpertiese, r.getPosition() + r.getSize() * (1/2.0f));
    } else if(drawType == 0) {
        PawnManagerClient::resourceRenderer->drawResource(res, r.getPosition() + r.getSize() * (1 / 2.0f), std::numbers::pi_v<float> / 4, 15);
    } else {
        PawnManagerClient::pawnRenderer->drawFighterPawn(fighterPawnType, r.getPosition() + r.getSize() * (1 / 2.0f));
    }
}
