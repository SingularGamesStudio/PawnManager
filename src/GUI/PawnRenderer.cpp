//
// Created by goodm on 16.03.2023.
//

#include "PawnRenderer.h"

#include <cmath>
#include <iostream>
#include <numbers>

#include "../Entities/Pawns/FighterPawn.h"
#include "../Entities/Pawns/WorkerPawn.h"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/Window.hpp"

void PawnRenderer::drawWorkerPawn(const std::set<expertisesID>& expertises, sf::Vector2f pos, float size) {
    std::vector<sf::Vector2f> vertices;
    std::vector<sf::Vector2f> outerVertices;
    int cnt = 30;
    for (int i = 0; i < cnt; ++i) {
        float angle = static_cast<float>(i) * 2 * std::numbers::pi_v<float> / cnt;
        float x = std::cos(angle);
        float y = std::sin(angle);
        vertices.push_back(sf::Vector2f(x * (size - 2), y * (size - 2)) + pos);
        outerVertices.push_back(sf::Vector2f(x * size, y * size) + pos);
    }
    sf::VertexArray arr;
    sf::Transform t;
    std::vector<expertisesID> expertisesVec(expertises.begin(), expertises.end());
    expertisesVec.resize(std::remove(expertisesVec.begin(), expertisesVec.end(), expertisesID::Nitwit) - expertisesVec.begin());
    arr.setPrimitiveType(sf::PrimitiveType::Triangles);
    for (int i = 0; i < vertices.size(); ++i) {
        sf::Color col;
        if (expertisesVec.empty()) {
            col = sf::Color(127, 127, 127);
        } else {
            int expertiseIndex = (i * static_cast<int>(expertisesVec.size())) / cnt;
            switch (expertisesVec[expertiseIndex]) {
                case expertisesID::Lumbering:
                    col = sf::Color::Green;
                    break;
                case expertisesID::Mining:
                    col = sf::Color(27, 27, 27);
                    break;
                case expertisesID::Smithing:
                    col = sf::Color(179, 20, 57);
                    break;
                case expertisesID::Nitwit:
                    col = sf::Color(127, 127, 127);
                    break;
            }
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

PawnRenderer::PawnRenderer(sf::RenderWindow& window) : window(window) {}

void PawnRenderer::drawFighterPawn(FighterPawnType type, sf::Vector2f pos, float size) {
    std::vector<sf::Vector2f> vertices;
    std::vector<sf::Vector2f> outerVertices;
    int cnt = 3;
    for (int i = 0; i < cnt; ++i) {
        float angle = static_cast<float>(i) * 2 * std::numbers::pi_v<float> / cnt;
        float x = std::cos(angle);
        float y = std::sin(angle);
        vertices.push_back(sf::Vector2f(x * (size - 2), y * (size - 2)) + pos);
        outerVertices.push_back(sf::Vector2f(x * size, y * size) + pos);
    }
    sf::VertexArray arr;
    sf::Transform t;
    sf::Color col;
    switch (type) {
        case FighterPawnType::Monk:
            col = sf::Color(100, 100, 100);
            break;
        case FighterPawnType::Swordsman:
            col = sf::Color(255, 143, 14);
            break;
    }
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
