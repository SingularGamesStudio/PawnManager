//
// Created by goodm on 30.03.2023.
//

#include "CraftBuildingWindow.h"
#include "../IDmanager.h"
#include "PawnManagerClient.h"
#include "SFML/Graphics/Text.hpp"
#include "ButtonControl.h"
#include "../Entities/Buildings/CraftBuilding.h"

CraftBuildingWindow::CraftBuildingWindow(int id) : id(id) {
    slotCounts = sf::Vector2i(5, 5);
    controls.push_back(new ButtonControl(*this, sf::IntRect(0, 4, 4, 0),
                                         "Assign recipe",
                                         [id=id](){
        ptr<CraftBuilding> p(id);
        Recipe* r = p->recipes[0];
        PawnManagerClient::player->manager.startRecipe(r,static_cast<ptr<Building>>(p));
        PawnManagerClient::winManager.popWindow();
    }));
}

void CraftBuildingWindow::updateAndRender() {
    if(IDmanager::get(id) == nullptr) {
        PawnManagerClient::winManager.popWindow();
        return;
    }
    GameWindow::updateAndRender();
    sf::FloatRect fr = getWindowRectangle();
    sf::Text t("Craft Building", PawnManagerClient::fontManager.f);
    t.setCharacterSize(20);
    t.setFillColor(sf::Color(0, 0, 0));
    t.setPosition(fr.getPosition() + sf::Vector2f(10, 10));
    PawnManagerClient::window->draw(t);
}

