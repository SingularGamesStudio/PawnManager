//
// Created by goodm on 13.04.2023.
//

#include "ArrowControl.h"

#include "GameWindow.h"
#include "PawnManagerClient.h"
#include "SFML/Graphics/VertexArray.hpp"

ArrowControl::ArrowControl(GameWindow& win, const sf::IntRect& rect) : Control(win, rect) {}

void ArrowControl::updateAndRender() {
    Control::updateAndRender();
    sf::FloatRect r = win.getGridRectangle(rect);
    float horizontalCenter = r.top + r.height / 2;
    float arrowTriangleMin = r.left + r.width - r.height / 2;
    sf::Color col(100, 100, 100);
    sf::VertexArray arr;
    arr.setPrimitiveType(sf::Triangles);
    arr.append(sf::Vertex(sf::Vector2f(r.left + r.width, horizontalCenter), col));
    arr.append(sf::Vertex(sf::Vector2f(arrowTriangleMin, r.top), col));
    arr.append(sf::Vertex(sf::Vector2f(arrowTriangleMin, r.top + r.height), col));
    PawnManagerClient::window->draw(arr);
    win.drawRect(sf::FloatRect(r.left, horizontalCenter - 10, arrowTriangleMin - r.left, 20), col);
    sf::Vector2f center = r.getPosition() + r.getSize() * (1 / 2.0f);
    for (int i = 0; i < pawnExpertises.size(); ++i) {
        float part = (pawnExpertises.size() == 1) ? 0.5f : ((float)i / (pawnExpertises.size() - 1));
        float delta = ((part * 2) - 1) * 40;
        PawnManagerClient::pawnRenderer->drawWorkerPawn({pawnExpertises[i]}, center + sf::Vector2f(delta, 0), 20);
    }
}
