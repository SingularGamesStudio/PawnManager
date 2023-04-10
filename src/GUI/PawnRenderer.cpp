//
// Created by goodm on 16.03.2023.
//

#include "PawnRenderer.h"
#include "SFML/Window.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "../Entities/Pawns/WorkerPawn.h"
#include <cmath>
#include <numbers>

void PawnRenderer::drawWorkerPawn(const std::set<expertisesID>& expertises, Vec2f pos) {
    std::vector<Vec2f> vertices;
    std::vector<Vec2f> outerVertices;
    int cnt = 30;
    for(int i = 0; i < cnt; ++i) {
        float angle = static_cast<float>(i) * 2 * std::numbers::pi_v<float> / cnt;
        float x = std::cos(angle);
        float y = std::sin(angle);
        vertices.push_back(Vec2f(x * 10.0f, y * 10.0f) + pos);
        outerVertices.push_back(Vec2f(x * 12.0f, y * 12.0f) + pos);
    }
    sf::VertexArray arr;
    sf::Transform t;
    std::vector<expertisesID> expertisesVec(expertises.begin(), expertises.end());
    arr.setPrimitiveType(sf::PrimitiveType::Triangles);
    for(int i = 0; i < vertices.size(); ++i) {
        sf::Color col;
        if(expertisesVec.empty()) {
            col = sf::Color(127, 127, 127);
        } else {
            int expertiseIndex = (i * static_cast<int>(expertisesVec.size())) / cnt;
            switch (expertisesVec[expertiseIndex]) {
                case expertisesID::DummySmeltery:
                    col = sf::Color::Red;
                    break;
                case expertisesID::DummtTrainership:
                    col = sf::Color::Green;
                    break;
                case expertisesID::DummyMetalworking:
                    col = sf::Color::White;
                    break;
            }
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

PawnRenderer::PawnRenderer(sf::RenderWindow& window) : window(window) {

}
