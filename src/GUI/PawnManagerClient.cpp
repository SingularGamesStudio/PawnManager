//
// Created by goodm on 16.03.2023.
//

#include "PawnManagerClient.h"
#include "SFML/Window.hpp"
#include "MainMenuWindow.h"
#include "../testSystem.h"
#include "../Entities/Pawns/Pawn.h"
#include "../Entities/Pawns/WorkerPawn.h"
#include "../Entities/Buildings/Building.h"
#include <random>
#include <ctime>

PawnManagerClient::PawnManagerClient() : window(sf::VideoMode(800, 600), "Pawn Manager"),
                                         view(window.getDefaultView()), winManager(), pawnRenderer(window),
                                         buildingRenderer(window), resourceRenderer(window) {
    winManager.pushWindow(new MainMenuWindow());
}

void PawnManagerClient::run() {
    player = initTest();
    while (window.isOpen()) {
        sf::Event evt{};
        while (window.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) {
                window.close();
            }
            if (evt.type == sf::Event::Resized) {
                view.setSize({
                                     static_cast<float>(evt.size.width),
                                     static_cast<float>(evt.size.height)
                             });
                view.setCenter(evt.size.width / 2, evt.size.height / 2);
                window.setView(view);
            }
        }
        updateAndRender();
        window.display();
    }
}

PawnManagerClient::~PawnManagerClient() {
    while (winManager.windowCount() > 0) {
        winManager.popWindow();
    }
}


void PawnManagerClient::updateAndRender() {
    double newTime = clock();
    tick((newTime-curTime)/CLOCKS_PER_SEC);
    curTime = newTime;
    window.clear(sf::Color::White);
    winManager.updateAndRender();
    Vec2f center = ((Vec2f )(Vector2<unsigned int>)window.getSize()) * 0.5f;
    buildingRenderDfs(player->hub, center);
    std::default_random_engine rng;
    std::uniform_real_distribution<float> dist2(0, std::numbers::pi_v<float>);
    for (Pawn* p: player->pawns) {
        auto [x, y] = p->position;
        auto* wp = dynamic_cast<WorkerPawn*>(p);
        if(wp != nullptr) {
            pawnRenderer.drawWorkerPawn(wp->expertises,Vec2f(x, y) * renderScale + center);
            if(wp->holding!=Resource::DummyNothing){
                float rotation = dist2(rng);
                resourceRenderer.drawResource(wp->holding, Vec2f(x, y) * renderScale + center, rotation);
            }
        }
    }
}

void PawnManagerClient::buildingRenderDfs(Building* b, Vec2f center) {
    auto [x, y] = b->position;
    for(Building* ob : b->children) {
        buildingRenderer.drawEdge(b, ob, center);
        buildingRenderDfs(ob, center);
    }
    Vec2f p(x, y);
    buildingRenderer.drawBuilding(b, p * renderScale + center);
    std::default_random_engine rng;
    std::uniform_real_distribution<float> dist(-1, 1);
    std::uniform_real_distribution<float> dist2(0, std::numbers::pi_v<float>);
    for(Resource res : b->resources) {
        float x = 2;
        float y = 2;
        while(x * x + y * y >= 1) {
            x = dist(rng);
            y = dist(rng);
        }
        float rotation = dist2(rng);
        Vec2f pos = (Vec2f(x, y) * (float)(b->radius - 7 / renderScale) + p) * renderScale + center;
        resourceRenderer.drawResource(res, pos, rotation);
    }
}
