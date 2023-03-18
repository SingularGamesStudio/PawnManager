#include "CraftBuilding.h"

void CraftBuilding::destroy() {}

void CraftBuilding::stopRecipe() {
    if(current== nullptr)
        return;
    current->cancel();
    current = nullptr;
}

bool CraftBuilding::assignRecipe(Recipe* recipe){
    stopRecipe();
    current = recipe;
    if(!current->checkRequirements(this))
        return false;
    current->start(this);
    return true;
}

void CraftBuilding::tick() {
    current->tick();
}