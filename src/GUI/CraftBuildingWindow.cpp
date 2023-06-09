//
// Created by goodm on 30.03.2023.
//

#include "CraftBuildingWindow.h"

#include "../Core/godobject.h"
#include "../Entities/Buildings/CraftBuilding.h"
#include "../Recipes/CraftRecipe.h"
#include "../Recipes/WorkerRecipe.h"
#include "ArrowControl.h"
#include "ButtonControl.h"
#include "PawnManagerClient.h"
#include "SFML/Graphics/Text.hpp"

CraftBuildingWindow::CraftBuildingWindow(int id) : id(id), selectedRecipe(0), shouldClose(false) {
    slotCounts = sf::Vector2i(7, 5);
    {
        ptr<CraftBuilding> p(id);
        if (p->current) { selectedRecipe = p->recipes.size(); }
    }
    controls.push_back(assignButton = new ButtonControl(*this, sf::IntRect(1, 4, 4, 0), "Assign recipe", [w = this]() {
                           ptr<CraftBuilding> p(w->id);
                           Recipe* r = w->getRecipe();

                           godObject::local_server->mainPlayer->localStart(r, p.dyn_cast<Building>());
                           w->shouldClose = true;
                           //        PawnManagerClient::winManager.popWindow();
                       }));
    controls.push_back(new ButtonControl(*this, sf::IntRect(0, 4, 0, 0), "<", [this]() {
        if (selectedRecipe == 0) {
            selectedRecipe = getRecipeCount() - 1;
        } else {
            --selectedRecipe;
        }
    }));
    controls.push_back(new ButtonControl(*this, sf::IntRect(6, 4, 0, 0), ">", [this]() {
        if (selectedRecipe == getRecipeCount() - 1) {
            selectedRecipe = 0;
        } else {
            ++selectedRecipe;
        }
    }));
    for (int j = 0; j < 6; ++j) {
        SlotControl* sc = new SlotControl(*this, sf::IntRect(j / 3, 1 + (j % 3), 0, 0));
        controls.push_back(sc);
        inputSlots.push_back(sc);
        SlotControl* sc2 = new SlotControl(*this, sf::IntRect(5 + (j / 3) , 1 + (j % 3), 0, 0));
        controls.push_back(sc2);
        outputSlots.push_back(sc2);
    }
    controls.push_back(arrow = new ArrowControl(*this, sf::IntRect(2, 2, 2, 0)));
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
    assignButton->disabled = isRecipeCurrent();
    GameWindow::updateAndRender();
    sf::FloatRect fr = getWindowRectangle();
    sf::Text t(std::string("Craft Building ") + (isRecipeCurrent() ? "Current recipe" : std::to_string(selectedRecipe)),
               PawnManagerClient::fontManager.f);
    t.setCharacterSize(20);
    t.setFillColor(sf::Color(0, 0, 0));
    t.setPosition(fr.getPosition() + sf::Vector2f(10, 10));
    PawnManagerClient::window->draw(t);
    Recipe* r = getRecipe();
    int cInputPos = 0;
    int cOutputPos = 0;
    for (Resource res: r->inResources) {
        inputSlots[cInputPos]->drawType = 0;
        inputSlots[cInputPos++]->res = res;
    }
    for (expertisesID res: r->inWorkers) {
        inputSlots[cInputPos]->drawType = 1;
        inputSlots[cInputPos++]->pawnExpertiese = {res};
    }
    for (FighterPawnType f: r->inFighters) {
        inputSlots[cInputPos]->drawType = 2;
        inputSlots[cInputPos++]->fighterPawnType = f;
    }
    if(r->getType() == RecipeType::CRAFT_RECIPE) {
        CraftRecipe *cr = dynamic_cast<CraftRecipe *>(r);
        for (Resource res: cr->outResources) {
            outputSlots[cOutputPos]->drawType = 0;
            outputSlots[cOutputPos++]->res = res;
        }
        for (FighterPawnType f: cr->outFighters) {
            outputSlots[cOutputPos]->drawType = 2;
            outputSlots[cOutputPos++]->fighterPawnType = f;
        }
    }
    if(r && r->getType() == RecipeType::WORKER_RECIPE) {
        WorkerRecipe *wr = dynamic_cast<WorkerRecipe *>(r);
        for (auto res: wr->outWorkers) {
            outputSlots[cOutputPos]->drawType = 1;
            std::set<expertisesID> st(res.begin(), res.end());
            outputSlots[cOutputPos++]->pawnExpertiese = st;
        }
        for (expertisesID exp: wr->trainExpertises) {
            outputSlots[cOutputPos]->drawType = 1;
            outputSlots[cOutputPos++]->pawnExpertiese = {exp};
        }
    }
    for (; cInputPos < inputSlots.size(); ++cInputPos) {
        inputSlots[cInputPos]->drawType = 0;
        inputSlots[cInputPos++]->res = Resource::Nothing;
    }
    for (; cOutputPos < outputSlots.size(); ++cOutputPos) {
        outputSlots[cOutputPos]->drawType = 0;
        outputSlots[cOutputPos]->res = Resource::Nothing;
    }
    arrow->pawnExpertises = r->reqWorkers;
}
int CraftBuildingWindow::getRecipeCount() {
    ptr<CraftBuilding> p(id);
    return p->recipes.size() + (p->current ? 1 : 0);
}
Recipe* CraftBuildingWindow::getRecipe() {
    if (selectedRecipe >= getRecipeCount()) { selectedRecipe = getRecipeCount() - 1; }
    ptr<CraftBuilding> p(id);
    if (p->recipes.size() == selectedRecipe) { return p->current; }
    return p->recipes[selectedRecipe];
}
bool CraftBuildingWindow::isRecipeCurrent() {
    ptr<CraftBuilding> p(id);
    return selectedRecipe == p->recipes.size();
}
