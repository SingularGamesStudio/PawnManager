//
// Created by goodm on 16.03.2023.
//

#include "GameWindow.h"
#include "PawnManagerClient.h"
#include "SFML/Graphics/VertexArray.hpp"

void GameWindow::updateAndRender() {
    drawBackground();
    drawWindow();
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

void GameWindow::drawWindow() {
    sf::FloatRect windowRect = getWindowRectangle();
    sf::Color c = sf::Color(192, 192, 192);
    sf::Color borderColor = sf::Color(0, 0, 0);
    drawBorderedRect(windowRect, 4, c, borderColor);
}

sf::Vector2f GameWindow::getMinGridCellPos() {
    auto [width, height] = client->window.getSize();
    auto center = sf::Vector2f(width, height) * (1 / 2.0f);
    auto gridSize = static_cast<sf::Vector2f>(slotCounts) * (gridCellSize + gridCellPadding) -
                    sf::Vector2f(gridCellPadding, gridCellPadding);
    return center - gridSize * (1 / 2.0f);
}

sf::FloatRect GameWindow::getGridCellRectangle(sf::Vector2i pos) {
    auto minGridCellPos = getMinGridCellPos();
    auto cellMinPos = minGridCellPos + static_cast<sf::Vector2f>(pos) * (gridCellSize + gridCellPadding);
    auto cellMaxPos = cellMinPos + sf::Vector2f(gridCellSize, gridCellSize);
    return {cellMinPos, cellMaxPos - cellMinPos};
}

sf::FloatRect GameWindow::getWindowRectangle() {
    auto min = getGridCellRectangle(sf::Vector2i(0, 0)).getPosition() -
               sf::Vector2f(windowSizeExtension, windowSizeExtension);
    auto maxRect = getGridCellRectangle(slotCounts - sf::Vector2i(1, 1));
    auto max = maxRect.getPosition() + maxRect.getSize() + sf::Vector2f(windowSizeExtension, windowSizeExtension);
    return {min, max - min};
}

void GameWindow::drawRect(sf::FloatRect rect, sf::Color c) {
    sf::VertexArray arr;
    arr.setPrimitiveType(sf::Triangles);
    auto top = rect.top;
    auto left = rect.left;
    auto bottom = top + rect.height;
    auto right = left + rect.width;
    arr.append(sf::Vertex(sf::Vector2f(left, top), c));
    arr.append(sf::Vertex(sf::Vector2f(right, top), c));
    arr.append(sf::Vertex(sf::Vector2f(right, bottom), c));
    arr.append(sf::Vertex(sf::Vector2f(left, top), c));
    arr.append(sf::Vertex(sf::Vector2f(right, bottom), c));
    arr.append(sf::Vertex(sf::Vector2f(left, bottom), c));
    client->window.draw(arr);
}

void GameWindow::drawBorderedRect(sf::FloatRect rect, float borderSize, sf::Color c, sf::Color border) {
    drawRect(rect, c);
    drawRect(sf::FloatRect(rect.left, rect.top, rect.width, borderSize), border);
    drawRect(sf::FloatRect(rect.left, rect.top, borderSize, rect.height), border);
    drawRect(sf::FloatRect(rect.left, rect.top + rect.height - borderSize, rect.width,
                           borderSize),border);
    drawRect(sf::FloatRect(rect.left + rect.width - borderSize, rect.top, borderSize,
                           rect.height),border);
}
