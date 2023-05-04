//
// Created by goodm on 16.03.2023.
//

#include "PawnManagerClient.h"

#include <ctime>
#include <iostream>
#include <numbers>
#include <random>

#include "../Entities/Buildings/Building.h"
#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/FighterPawn.h"
#include "../Entities/Pawns/WorkerPawn.h"
#include "../Entities/ResourceEntity.h"
#include "../Recipes/BuildRecipe.h"
#include "../testSystem.h"
#include "BuildBuildingWindow.h"
#include "CraftBuildingWindow.h"
#include "MainMenuWindow.h"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Window.hpp"


sf::RenderWindow* PawnManagerClient::window;
sf::View PawnManagerClient::view;
PawnRenderer* PawnManagerClient::pawnRenderer;
BuildingRenderer* PawnManagerClient::buildingRenderer;
ResourceRenderer* PawnManagerClient::resourceRenderer;
ptr<Player> PawnManagerClient::player;
double PawnManagerClient::curTime;
int PawnManagerClient::selectedBuilding;
GameWindowManager PawnManagerClient::winManager;
FontManager PawnManagerClient::fontManager;


void PawnManagerClient::run() {
    init();
    player = initTest();
    while (window->isOpen()) {
        sf::Event evt{};
        while (window->pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) { window->close(); }
            if (evt.type == sf::Event::Resized) {
                view.setSize({static_cast<float>(evt.size.width), static_cast<float>(evt.size.height)});
                view.setCenter(evt.size.width / 2, evt.size.height / 2);
                window->setView(view);
            }
            if (evt.type == sf::Event::MouseButtonPressed) { onMouseClick(evt.mouseButton.x, evt.mouseButton.y, evt.mouseButton.button); }
            if (evt.type == sf::Event::KeyPressed) {
                if (evt.key.code == sf::Keyboard::Escape) {
                    if (winManager.windowCount() > 0) { winManager.popWindow(); }
                } else if(winManager.windowCount() > 0) {
                    winManager.onKeyInput(evt.key.code);
                }
            }
            if(evt.type == sf::Event::TextEntered) {
                if(winManager.windowCount() > 0) {
                    winManager.onCharInput(evt.text.unicode);
                }
            }
        }
        updateAndRender();
        window->display();
    }
    shutdown();
}


void PawnManagerClient::updateAndRender() {
    double newTime = clock();
    tick((newTime - curTime) / CLOCKS_PER_SEC);
    curTime = newTime;
    window->clear(sf::Color::White);
    sf::Vector2f center = ((sf::Vector2f) window->getSize()) * 0.5f;
    buildingRenderDfs(player->hub, center);
    std::default_random_engine rng;
    std::uniform_real_distribution<float> dist2(0, std::numbers::pi_v<float>);
    for (ptr<Pawn> p: player->pawns) {
        auto [x, y] = p->position;
        ptr<WorkerPawn> wp = p.dyn_cast<WorkerPawn>();
        ptr<FighterPawn> fp = p.dyn_cast<FighterPawn>();
        if (wp) {
            pawnRenderer->drawWorkerPawn(wp->expertises, sf::Vector2f(x, y) * renderScale + center);
            if (wp->holding != Resource::DummyNothing) {
                float rotation = dist2(rng);
                resourceRenderer->drawResource(wp->holding, sf::Vector2f(x, y) * renderScale + center, rotation);
            }
        } else if (fp) {
            pawnRenderer->drawFighterPawn(fp->getType(), sf::Vector2f(x, y) * renderScale + center);
        }
    }
    for (ptr<ResourceEntity> res: ResourceEntity::danglingResources) {
        resourceRenderer->drawResource(res->resource, sf::Vector2f(res->position.first, res->position.second) * renderScale + center,
                                       std::numbers::pi_v<float> / 4.0f);
    }
    winManager.updateAndRender();
}

void PawnManagerClient::buildingRenderDfs(ptr<Building> b, sf::Vector2f center) {
    auto [x, y] = b->position;
    for (ptr<Building> ob: b->children) {
        buildingRenderer->drawEdge(b, ob, center);
        buildingRenderDfs(ob, center);
    }
    sf::Vector2f p(x, y);
    buildingRenderer->drawBuilding(b, p * renderScale + center);
    std::default_random_engine rng;
    std::uniform_real_distribution<float> dist(-1, 1);
    std::uniform_real_distribution<float> dist2(0, std::numbers::pi_v<float>);
    for (Resource res: b->resources) {
        float x = 2;
        float y = 2;
        while (x * x + y * y >= 1) {
            x = dist(rng);
            y = dist(rng);
        }
        float rotation = dist2(rng);
        sf::Vector2f pos = (sf::Vector2f(x, y) * (float) (b->radius - 7 / renderScale) + p) * renderScale + center;
        resourceRenderer->drawResource(res, pos, rotation);
    }
    for (Resource res: b->reservedResources) {
        float x = 2;
        float y = 2;
        while (x * x + y * y >= 1) {
            x = dist(rng);
            y = dist(rng);
        }
        float rotation = dist2(rng);
        sf::Vector2f pos = (sf::Vector2f(x, y) * (float) (b->radius - 7 / renderScale) + p) * renderScale + center;
        resourceRenderer->drawResource(res, pos, rotation);
    }
}

void PawnManagerClient::onMouseClick(int x, int y, sf::Mouse::Button b) {
    if (winManager.windowCount() > 0) {
        winManager.onMouseClick(x, y, b);
        return;
    }
    sf::Vector2f center = ((sf::Vector2f) window->getSize()) * 0.5f;
    sf::Vector2f pos = (sf::Vector2f(x, y) - center) / renderScale;
    if (!onBuildingMouseClick(player->hub, pos, b)) {
        ptr<Building> building = ptr<Building>(selectedBuilding);
        if (building) {
            winManager.pushWindow(new BuildBuildingWindow(selectedBuilding, pos));
            selectedBuilding = -1;
        }
    }
}

bool PawnManagerClient::onBuildingMouseClick(ptr<Building> b, sf::Vector2f pos, sf::Mouse::Button button) {
    auto [x, y] = b->position;
    sf::Vector2f delta = sf::Vector2f(x, y) - pos;
    if (delta.x * delta.x + delta.y * delta.y <= b->radius * b->radius) {
        if (button == sf::Mouse::Left) {
            selectedBuilding = b->id;
        } else if (button == sf::Mouse::Right) {
            ptr<Building> c = b.dyn_cast<Building>();
            ptr<CraftBuilding> cb = c.dyn_cast<CraftBuilding>();
            if (cb && !cb->recipes.empty()) { winManager.pushWindow(new CraftBuildingWindow(c->id)); }
        } else if (button == sf::Mouse::Middle) {
            //b.del();
            player->attack(b);
        }
        return true;
    }
    for (ptr<Building> ob: b->children) {
        if (onBuildingMouseClick(ob, pos, button)) { return true; }
    }
    return false;
}

void PawnManagerClient::init() {
    window = new sf::RenderWindow(sf::VideoMode(800, 600), "Pawn Manager");
    view = window->getDefaultView();
    winManager = GameWindowManager();
    pawnRenderer = new PawnRenderer(*window);
    buildingRenderer = new BuildingRenderer(*window);
    resourceRenderer = new ResourceRenderer(*window);
    selectedBuilding = -1;
    fontManager = FontManager();
    winManager.pushWindow(new MainMenuWindow());
}

void PawnManagerClient::shutdown() {
    while (winManager.windowCount() > 0) { winManager.popWindow(); }
    delete resourceRenderer;
    delete buildingRenderer;
    delete pawnRenderer;
    delete window;
}
