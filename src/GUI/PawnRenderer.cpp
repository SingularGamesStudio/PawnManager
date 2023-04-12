//
// Created by goodm on 16.03.2023.
//

#include "PawnRenderer.h"

#include <cmath>
#include <numbers>

#include "../Entities/Pawns/WorkerPawn.h"
#include "VertexArray.h"

void PawnRenderer::drawWorkerPawn(const std::set<expertisesID>& expertises, Vec2f pos) {
    std::vector<Vec2f> vertices;
    std::vector<Vec2f> outerVertices;
    int cnt = 30;
    for (int i = 0; i < cnt; ++i) {
        float angle = static_cast<float>(i) * 2 * std::numbers::pi_v<float> / cnt;
        float x = std::cos(angle);
        float y = std::sin(angle);
        vertices.push_back(Vec2f(x * 10.0f, y * 10.0f) + pos);
        outerVertices.push_back(Vec2f(x * 12.0f, y * 12.0f) + pos);
    }
    VertexArray arr;
    std::vector<expertisesID> expertisesVec(expertises.begin(), expertises.end());
    for (int i = 0; i < vertices.size(); ++i) {
        Color col;
        if (expertisesVec.empty()) {
            col = Color(127, 127, 127);
        } else {
            int expertiseIndex = (i * static_cast<int>(expertisesVec.size())) / cnt;
            switch (expertisesVec[expertiseIndex]) {
                case expertisesID::DummySmeltery:
                    col = Color::Red;
                    break;
                case expertisesID::DummtTrainership:
                    col = Color::Green;
                    break;
                case expertisesID::DummyMetalworking:
                    col = Color::White;
                    break;
            }
        }
        int j = i + 1;
        if (j == vertices.size()) { j = 0; }
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
    window.draw(arr);
}

PawnRenderer::PawnRenderer(RenderTarget& window) : window(window) {}
