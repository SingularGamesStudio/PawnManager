//
// Created by goodm on 16.03.2023.
//

#include "ResourceRenderer.h"

#include <cmath>
#include <numbers>

#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Window.hpp"

void ResourceRenderer::drawResource(Resource r, sf::Vector2f pos, float rotation, float sz) {
    if(r == Resource::Nothing) {
        return;
    }
    std::vector<sf::Vector2f> vertices;
    std::vector<sf::Vector2f> outerVertices;
    int cnt = 3;
    switch (r) {
        case Resource::Steel:
            cnt = 4;
            break;
        case Resource::Stone:
            cnt = 4;
            break;
        case Resource::Ore:
            cnt = 5;
            break;
        case Resource::Wood:
            cnt = 6;
            break;
    }
    for (int i = 0; i < cnt; ++i) {
        float angle = static_cast<float>(i) * 2 * std::numbers::pi_v<float> / cnt + rotation;
        float x = std::cos(angle);
        float y = std::sin(angle);
        vertices.push_back(sf::Vector2f(x * (sz - 2), y * (sz - 2)) + pos);
        outerVertices.push_back(sf::Vector2f(x * sz, y * sz) + pos);
    }
    sf::VertexArray arr;
    sf::Transform t;
    arr.setPrimitiveType(sf::PrimitiveType::Triangles);
    for (int i = 0; i < vertices.size(); ++i) {
        sf::Color col = sf::Color::Red;
        switch (r) {

            case Resource::Ore:
                col = sf::Color(192, 192, 192);
                break;
            case Resource::Steel:
                col = sf::Color(212, 212, 212);
                break;
            case Resource::Wood:
                col = sf::Color(143, 94, 26);
                break;
            case Resource::Stone:
                col = sf::Color(93, 93, 93);
                break;
        }
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

ResourceRenderer::ResourceRenderer(sf::RenderWindow& window) : window(window) {}
