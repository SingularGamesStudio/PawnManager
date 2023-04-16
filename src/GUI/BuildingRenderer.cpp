//
// Created by goodm on 18.03.2023.
//

#include "BuildingRenderer.h"

#include <cmath>

#include "../Entities/Buildings/Building.h"
#include "../Entities/Buildings/CraftBuilding.h"
#include "PawnManagerClient.h"
#include "SFML/Graphics/VertexArray.hpp"

BuildingRenderer::BuildingRenderer(sf::RenderWindow& window) : window(window) {}

void BuildingRenderer::drawBuilding(ptr<Building> b, sf::Vector2f pos) {
    ptr<CraftBuilding> cb = b.dyn_cast<CraftBuilding>();
    if(cb) {
        if(cb->recipes.size() == 0) {
            drawBuilding(cb->radius, sf::Color::Blue, pos);
        } else {
            drawBuilding(cb->radius, sf::Color::Green, pos);
        }
    } else {
        drawBuilding(b->radius, sf::Color::White, pos);
    }
}

void BuildingRenderer::drawEdge(ptr<Building> a, ptr<Building> b, sf::Vector2f center) {
    sf::Vector2f aPos = sf::Vector2f(a->position.first, a->position.second) * PawnManagerClient::renderScale + center;
    sf::Vector2f bPos = sf::Vector2f(b->position.first, b->position.second) * PawnManagerClient::renderScale + center;
    sf::Vector2f delta = bPos - aPos;
    sf::Vector2f orth(delta.y, -delta.x);
    orth *= (5.0f / sqrtf(orth.x * orth.x + orth.y * orth.y));
    sf::VertexArray arr;
    arr.setPrimitiveType(sf::Triangles);
    sf::Color c(127, 127, 127);
    arr.append(sf::Vertex(aPos + orth, c));
    arr.append(sf::Vertex(bPos + orth, c));
    arr.append(sf::Vertex(bPos - orth, c));
    arr.append(sf::Vertex(aPos + orth, c));
    arr.append(sf::Vertex(bPos - orth, c));
    arr.append(sf::Vertex(aPos - orth, c));
    window.draw(arr);
}
void BuildingRenderer::drawBuilding(float radius, sf::Color col, sf::Vector2f pos) {
    radius *= PawnManagerClient::renderScale;
    std::vector<sf::Vector2f> vertices;
    std::vector<sf::Vector2f> outerVertices;
    int cnt = 30;
    for (int i = 0; i < cnt; ++i) {
        float angle = static_cast<float>(i) * 2 * std::numbers::pi_v<float> / cnt;
        float x = std::cos(angle);
        float y = std::sin(angle);
        vertices.push_back(sf::Vector2f(x * (radius - 2), y * (radius - 2)) + pos);
        outerVertices.push_back(sf::Vector2f(x * radius, y * radius) + pos);
    }
    sf::VertexArray arr;
    sf::Transform t;
    arr.setPrimitiveType(sf::PrimitiveType::Triangles);
    for (int i = 0; i < vertices.size(); ++i) {
        int j = i + 1;
        if (j == vertices.size()) { j = 0; }
        arr.append(sf::Vertex(vertices[j], col));
        arr.append(sf::Vertex(vertices[i], col));
        arr.append(sf::Vertex(pos, col));
        arr.append(sf::Vertex(vertices[j], sf::Color::Black));
        arr.append(sf::Vertex(outerVertices[j], sf::Color::Black));
        arr.append(sf::Vertex(outerVertices[i], sf::Color::Black));
        arr.append(sf::Vertex(vertices[j], sf::Color::Black));
        arr.append(sf::Vertex(outerVertices[i], sf::Color::Black));
        arr.append(sf::Vertex(vertices[i], sf::Color::Black));
    }
    window.draw(arr);
}

void BuildingRenderer::drawBuilding(BuildingIdea& idea, sf::Vector2f pos) {
    if(idea.available.empty()) {
        drawBuilding(idea.radius, sf::Color::Green, pos);
    } else {
        drawBuilding(idea.radius, sf::Color::White, pos);
    }
}
