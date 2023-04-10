//
// Created by goodm on 18.03.2023.
//

#include "BuildingRenderer.h"
#include "SFML/Graphics/VertexArray.hpp"
#include "../Entities/Buildings/Building.h"
#include "PawnManagerClient.h"
#include <cmath>
#include "../Entities/Buildings/CraftBuilding.h"

BuildingRenderer::BuildingRenderer(sf::RenderWindow& window) : window(window) {

}

void BuildingRenderer::drawBuilding(Building* b, Vec2f pos) {
    std::vector<Vec2f> vertices;
    std::vector<Vec2f> outerVertices;
    int cnt = 30;
    float radius = b->radius * PawnManagerClient::renderScale;
    for(int i = 0; i < cnt; ++i) {
        float angle = static_cast<float>(i) * 2 * std::numbers::pi_v<float> / cnt;
        float x = std::cos(angle);
        float y = std::sin(angle);
        vertices.push_back(Vec2f(x * (radius - 2), y * (radius - 2)) + pos);
        outerVertices.push_back(Vec2f(x * radius, y * radius) + pos);
    }
    sf::VertexArray arr;
    sf::Transform t;
    arr.setPrimitiveType(sf::PrimitiveType::Triangles);
    for(int i = 0; i < vertices.size(); ++i) {
        sf::Color col = sf::Color::White;
        if(dynamic_cast<CraftBuilding*>(b) != nullptr) {
            col = sf::Color::Green;
        }
        int j = i + 1;
        if(j == vertices.size()) {
            j = 0;
        }
        arr.append(sf::Vertex((sf::Vector2f)vertices[j], col));
        arr.append(sf::Vertex((sf::Vector2f)vertices[i], col));
        arr.append(sf::Vertex((sf::Vector2f)pos, col));
        arr.append(sf::Vertex((sf::Vector2f)vertices[j], sf::Color::Black));
        arr.append(sf::Vertex((sf::Vector2f)outerVertices[j], sf::Color::Black));
        arr.append(sf::Vertex((sf::Vector2f)outerVertices[i], sf::Color::Black));
        arr.append(sf::Vertex((sf::Vector2f)vertices[j], sf::Color::Black));
        arr.append(sf::Vertex((sf::Vector2f)outerVertices[i], sf::Color::Black));
        arr.append(sf::Vertex((sf::Vector2f)vertices[i], sf::Color::Black));
    }
    window.draw(arr);
}

void BuildingRenderer::drawEdge(Building* a, Building* b, Vec2f center) {
    Vec2f aPos = Vec2f(a->position.first, a->position.second) * PawnManagerClient::renderScale + center;
    Vec2f bPos = Vec2f(b->position.first, b->position.second) * PawnManagerClient::renderScale + center;
    Vec2f delta = bPos - aPos;
    Vec2f orth(delta.y, -delta.x);
    float l = abs(orth);
    orth *= (5.0f / l);
    sf::VertexArray arr;
    arr.setPrimitiveType(sf::Triangles);
    sf::Color c(127, 127, 127);
    arr.append(sf::Vertex((sf::Vector2f)(aPos + orth), c));
    arr.append(sf::Vertex((sf::Vector2f)(bPos + orth), c));
    arr.append(sf::Vertex((sf::Vector2f)(bPos - orth), c));
    arr.append(sf::Vertex((sf::Vector2f)(aPos + orth), c));
    arr.append(sf::Vertex((sf::Vector2f)(bPos - orth), c));
    arr.append(sf::Vertex((sf::Vector2f)(aPos - orth), c));
    window.draw(arr);
}
