#include "CraftBuilding.h"

void CraftBuilding::destroy() {}

void CraftBuilding::stopRecipe() {
    progress = 0;
    current = Recipe::none();
}

void CraftBuilding::assignRecipe(Recipe recipe){
    stopRecipe();
    current = recipe;
}