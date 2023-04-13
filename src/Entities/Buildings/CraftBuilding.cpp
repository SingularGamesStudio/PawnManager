#include "CraftBuilding.h"

#include <iostream>

void CraftBuilding::stopRecipe() {
    if (current == nullptr) return;
    current->cancel();
    delete current;
    current = nullptr;
    zerocatcher();
}

bool CraftBuilding::assignRecipe(Recipe* recipe) {
    stopRecipe();
    if (!recipe->checkRequirements(ptr<CraftBuilding>(id))) return false;
    current = recipe->clone();
    current->start(ptr<CraftBuilding>(id));
    zerocatcher();
    return true;
}

void CraftBuilding::tick(double deltaTime) {
    zerocatcher();
    if (current != nullptr) { current->tick(deltaTime); }
}

CraftBuilding::~CraftBuilding() { stopRecipe(); }