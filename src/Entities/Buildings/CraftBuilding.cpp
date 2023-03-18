#include "CraftBuilding.h"

CraftBuilding::CraftBuilding(std::pair<double, double> pos, Player* owner, unsigned int hp, double radius, Building* parent):
Building(pos, owner, hp, radius, parent), current(nullptr) {
}

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
    if(current != nullptr) {
        current->tick();
    }
}