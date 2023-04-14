#include "CraftBuilding.h"

#include <iostream>
#ifdef SERVER_SIDE
void CraftBuilding::stopRecipe() {
    if (current == nullptr) return;
    current->cancel();
    delete current;
    current = nullptr;
}

bool CraftBuilding::assignRecipe(Recipe* recipe) {
    stopRecipe();
    if (!recipe->checkRequirements(ptr<CraftBuilding>(id))) return false;
    current = recipe->clone();
    current->start(ptr<CraftBuilding>(id));
    return true;
}

void CraftBuilding::tick(double deltaTime) {
    if (current != nullptr) { current->tick(deltaTime); }
}
#endif
CraftBuilding::~CraftBuilding() {
#ifdef SERVER_SIDE
    stopRecipe();
#endif
}