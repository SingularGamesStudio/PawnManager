//
// Created by goodm on 30.03.2023.
//

#include "CraftBuildingWindow.h"

#include "../Entities/Buildings/CraftBuilding.h"
#include "../Recipes/CraftRecipe.h"
#include "ArrowControl.h"
#include "ButtonControl.h"
#include "PawnManagerClient.h"
#include "SFML/Graphics/Text.hpp"

CraftBuildingWindow::CraftBuildingWindow(int id) : id(id), selectedRecipe(0), shouldClose(false) {
    slotCounts = sf::Vector2i(5, 5);
    controls.push_back(new ButtonControl(*this, sf::IntRect(1, 4, 2, 0), "Assign recipe", [w = this]() {
        ptr<CraftBuilding> p(w->id);
        Recipe* r = p->recipes[w->selectedRecipe];
        
        PawnManagerClient::player->manager.startRecipe(r, p.dyn_cast<Building>());
        w->shouldClose = true;
        //        PawnManagerClient::winManager.popWindow();
    }));
    controls.push_back(new ButtonControl(*this, sf::IntRect(0, 4, 0, 0), "<", [id = id, &selectedRecipe = selectedRecipe]() {
        ptr<CraftBuilding> p(id);
        if (selectedRecipe == 0) {
            selectedRecipe = p->recipes.size() - 1;
        } else {
            --selectedRecipe;
        }
    }));
    controls.push_back(new ButtonControl(*this, sf::IntRect(4, 4, 0, 0), ">", [id = id, &selectedRecipe = selectedRecipe]() {
        ptr<CraftBuilding> p(id);
        if (selectedRecipe == p->recipes.size() - 1) {
            selectedRecipe = 0;
        } else {
            ++selectedRecipe;
        }
    }));
    for (int j = 0; j < 3; ++j) {
        SlotControl* sc = new SlotControl(*this, sf::IntRect(0, 1 + j, 0, 0));
        controls.push_back(sc);
        inputSlots.push_back(sc);
        SlotControl* sc2 = new SlotControl(*this, sf::IntRect(4, 1 + j, 0, 0));
        controls.push_back(sc2);
        outputSlots.push_back(sc2);
    }
    controls.push_back(arrow = new ArrowControl(*this, sf::IntRect(1, 2, 2, 0)));
}

void CraftBuildingWindow::updateAndRender() {
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
    sf::Text t(std::string("Craft Building ") + std::to_string(selectedRecipe), PawnManagerClient::fontManager.f);
    t.setCharacterSize(20);
    t.setFillColor(sf::Color(0, 0, 0));
    t.setPosition(fr.getPosition() + sf::Vector2f(10, 10));
    PawnManagerClient::window->draw(t);
    ptr<CraftBuilding> cb(id);
    Recipe* r = cb->recipes[selectedRecipe];
    int cInputPos = 0;
    int cOutputPos = 0;
    CraftRecipe* cr = dynamic_cast<CraftRecipe*>(r);
    if (cr) {
        for (Resource res: cr->inResources) {
            inputSlots[cInputPos]->drawType = 0;
            inputSlots[cInputPos++]->res = res;
        }
        for (Resource res: cr->outResources) {
            outputSlots[cOutputPos]->drawType = 0;
            outputSlots[cOutputPos++]->res = res;
        }
        for(expertisesID res : cr->inWorkers) {
            inputSlots[cInputPos]->drawType = 1;
            inputSlots[cInputPos++]->pawnExpertiese = {res};
        }
        for(FighterPawnType f : cr->inFighters) {
            inputSlots[cInputPos]->drawType = 2;
            inputSlots[cInputPos++]->fighterPawnType = f;
        }
        for(FighterPawnType f : cr->outFighters) {
            outputSlots[cOutputPos]->drawType = 2;
            outputSlots[cOutputPos++]->fighterPawnType = f;
        }
    }
    for (; cInputPos < inputSlots.size(); ++cInputPos) {
        inputSlots[cInputPos]->drawType = 0;
        inputSlots[cInputPos++]->res = Resource::DummyNothing;
    }
    for (; cOutputPos < outputSlots.size(); ++cOutputPos) {
        outputSlots[cOutputPos]->drawType = 0;
        outputSlots[cOutputPos]->res = Resource::DummyNothing;
    }
    arrow->pawnExpertises = r->reqWorkers;
}
