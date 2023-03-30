//
// Created by goodm on 16.03.2023.
//

#include "GameWindow.h"
#include "PawnManagerClient.h"
#include "SFML/Graphics/VertexArray.hpp"

void GameWindow::updateAndRender() {
    drawBackground();
}

GameWindow::GameWindow(PawnManagerClient* client) : client(client) {}

void GameWindow::drawBackground() {
    sf::VertexArray arr;
    arr.setPrimitiveType(sf::Triangles);
    sf::Color c = sf::Color(0, 0, 0, 127);
    sf::Vector2u sz = client->window.getSize();
    arr.append(sf::Vertex(sf::Vector2f(0, 0), c));
    arr.append(sf::Vertex(sf::Vector2f(sz.x, 0), c));
    arr.append(sf::Vertex(sf::Vector2f(sz.x, sz.y), c));
    arr.append(sf::Vertex(sf::Vector2f(0, 0), c));
    arr.append(sf::Vertex(sf::Vector2f(sz.x, sz.y), c));
    arr.append(sf::Vertex(sf::Vector2f(0, sz.y), c));
    sf::RenderStates states = sf::RenderStates::Default;
    states.blendMode = sf::BlendAlpha;
    client->window.draw(arr, states);
}
