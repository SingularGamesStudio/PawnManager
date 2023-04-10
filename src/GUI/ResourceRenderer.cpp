//
// Created by goodm on 16.03.2023.
//

#include "ResourceRenderer.h"
#include "SFML/Window.hpp"
#include "VertexArray.h"
#include "../Entities/Pawns/WorkerPawn.h"
#include <cmath>
#include <numbers>

void ResourceRenderer::drawResource(Resource r, Vec2f pos, float rotation) {
    std::vector<Vec2f> vertices;
    std::vector<Vec2f> outerVertices;
    int cnt = 3;
    switch (r) {
        case Resource::DummyIngot:
            cnt = 4;
            break;
        case Resource::DummyOre:
            cnt = 5;
            break;
        case Resource::DummyWeapon:
            cnt = 4;
            break;
    }
    for(int i = 0; i < cnt; ++i) {
        float angle = static_cast<float>(i) * 2 * std::numbers::pi_v<float> / cnt + rotation;
        float x = std::cos(angle);
        float y = std::sin(angle);
        vertices.push_back(Vec2f(x * 5.0f, y * 5.0f) + pos);
        outerVertices.push_back(Vec2f(x * 7.0f, y * 7.0f) + pos);
    }
    VertexArray arr;
    sf::Transform t;
    for(int i = 0; i < vertices.size(); ++i) {
        Color col = Color::Red;
        switch (r) {

            case Resource::DummyOre:
                col = Color(192, 192, 192);
                break;
            case Resource::DummyIngot:
                col = Color(255, 255, 255);
                break;
            case Resource::DummyWeapon:
                col = Color::Red;
                break;
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

ResourceRenderer::ResourceRenderer(sf::RenderWindow& window) : window(window) {

}
