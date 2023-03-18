//
// Created by goodm on 16.03.2023.
//

#include "ResourceRenderer.h"
#include "SFML/Window.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "../Entities/Pawns/WorkerPawn.h"
#include <cmath>
#include <numbers>

void ResourceRenderer::drawResource(Resource r, sf::Vector2f pos, float rotation) {
    std::vector<sf::Vector2f> vertices;
    std::vector<sf::Vector2f> outerVertices;
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
        vertices.push_back(sf::Vector2f(x * 5.0f, y * 5.0f) + pos);
        outerVertices.push_back(sf::Vector2f(x * 7.0f, y * 7.0f) + pos);
    }
    sf::VertexArray arr;
    sf::Transform t;
    arr.setPrimitiveType(sf::PrimitiveType::Triangles);
    for(int i = 0; i < vertices.size(); ++i) {
        sf::Color col = sf::Color::Red;
        switch (r) {

            case Resource::DummyOre:
                col = sf::Color(192, 192, 192);
                break;
            case Resource::DummyIngot:
                col = sf::Color(255, 255, 255);
                break;
            case Resource::DummyWeapon:
                col = sf::Color::Red;
                break;
        }
        int j = i + 1;
        if(j == vertices.size()) {
            j = 0;
        }
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

ResourceRenderer::ResourceRenderer(sf::RenderWindow& window) : window(window) {

}
