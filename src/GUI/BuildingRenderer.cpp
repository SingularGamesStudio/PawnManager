//
// Created by goodm on 18.03.2023.
//

#include "BuildingRenderer.h"
#include "VertexArray.h"
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
    VertexArray arr;
    sf::Transform t;
    for(int i = 0; i < vertices.size(); ++i) {
        Color col = Color::White;
        if(dynamic_cast<CraftBuilding*>(b) != nullptr) {
            col = Color::Green;
        }
        int j = i + 1;
        if(j == vertices.size()) {
            j = 0;
        }
        arr.appendVertex(vertices[j], col);
        arr.appendVertex(vertices[i], col);
        arr.appendVertex(pos, col);
        arr.appendVertex(vertices[j], Color::Black);
        arr.appendVertex(outerVertices[j], Color::Black);
        arr.appendVertex(outerVertices[i], Color::Black);
        arr.appendVertex(vertices[j], Color::Black);
        arr.appendVertex(outerVertices[i], Color::Black);
        arr.appendVertex(vertices[i], Color::Black);
    }
    arr.draw(window);
}

void BuildingRenderer::drawEdge(Building* a, Building* b, Vec2f center) {
    Vec2f aPos = Vec2f(a->position.first, a->position.second) * PawnManagerClient::renderScale + center;
    Vec2f bPos = Vec2f(b->position.first, b->position.second) * PawnManagerClient::renderScale + center;
    Vec2f delta = bPos - aPos;
    Vec2f orth(delta.y, -delta.x);
    float l = abs(orth);
    orth *= (5.0f / l);
    VertexArray arr;
    Color c(127, 127, 127);
    arr.appendVertex((aPos + orth), c);
    arr.appendVertex((bPos + orth), c);
    arr.appendVertex((bPos - orth), c);
    arr.appendVertex((aPos + orth), c);
    arr.appendVertex((bPos - orth), c);
    arr.appendVertex((aPos - orth), c);
    arr.draw(window);
}
