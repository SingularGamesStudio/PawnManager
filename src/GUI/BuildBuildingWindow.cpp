//
// Created by goodm on 13.04.2023.
//

#include "BuildBuildingWindow.h"

#include "../Entities/Buildings/BuildingRegisty.h"
#include "../Recipes/BuildRecipe.h"
#include "ButtonControl.h"
#include "PawnManagerClient.h"
#include "SFML/Graphics/Text.hpp"


BuildBuildingWindow::BuildBuildingWindow(int id, const sf::Vector2f& pos) : id(id), pos(pos), selectedRecipe(0), shouldClose(false), arrow(nullptr) {

    slotCounts = sf::Vector2i(5, 5);
    controls.push_back(new ButtonControl(*this, sf::IntRect(1, 4, 2, 0), "Build", [w = this]() {
        //TODO
        //        ptr<CraftBuilding> p(w->id);
        //        Recipe* r = p->recipes[w->selectedRecipe];
        //        PawnManagerClient::player->manager.startRecipe(r, static_cast<ptr<Building>>(p));
        BuildRecipe* rec = dynamic_cast<BuildRecipe*>(BuildingRegisty::database[0]->clone());
        rec->pos = {w->pos.x, w->pos.y};
        PawnManagerClient::player->manager.startRecipe(rec, ptr<Building>(w->id));
        delete rec;
        w->shouldClose = true;
        //        PawnManagerClient::winManager.popWindow();
    }));
    controls.push_back(new ButtonControl(*this, sf::IntRect(0, 4, 0, 0), "<", [id = id, &selectedRecipe = selectedRecipe]() {
        ptr<CraftBuilding> p(id);
        if (selectedRecipe == 0) {
            //TODO
            //            selectedRecipe = p->recipes.size() - 1;
        } else {
            --selectedRecipe;
        }
    }));
    controls.push_back(new ButtonControl(*this, sf::IntRect(4, 4, 0, 0), ">", [id = id, &selectedRecipe = selectedRecipe]() {
        ptr<CraftBuilding> p(id);
        //TODO
        //        if (selectedRecipe == p->recipes.size() - 1) {
        selectedRecipe = 0;
        //        } else {
        //            ++selectedRecipe;
        //        }
    }));
    for (int j = 0; j < 3; ++j) {
        SlotControl* sc = new SlotControl(*this, sf::IntRect(0, 1 + j, 0, 0));
        controls.push_back(sc);
        inputSlots.push_back(sc);
    }
    controls.push_back(arrow = new ArrowControl(*this, sf::IntRect(1, 2, 2, 0)));
}

void BuildBuildingWindow::updateAndRender() {
    if (shouldClose) {
        PawnManagerClient::winManager.popWindow();
        return;
    }
    if (IDmanager::get(id) == nullptr) {
        PawnManagerClient::winManager.popWindow();
        return;
    }
    GameWindow::updateAndRender();
    sf::FloatRect fr = getWindowRectangle();
    sf::Text t(std::string("Build Building ") + std::to_string(selectedRecipe), PawnManagerClient::fontManager.f);
    t.setCharacterSize(20);
    t.setFillColor(sf::Color(0, 0, 0));
    t.setPosition(fr.getPosition() + sf::Vector2f(10, 10));
    PawnManagerClient::window->draw(t);
}
