
#include "BuildingRegisty.h"

#include "../../Recipes/CraftRecipe.h"
#include "Building.h"
std::vector<BuildingIdea> BuildingRegisty::database;

void BuildingRegisty::init() {
    database.push_back(BuildingIdea(Building::baseBuildingRadius, database.size()));
    database.push_back(BuildingIdea(Building::baseBuildingRadius * 2, database.size()));
    CraftRecipe* recipe = new CraftRecipe();
    recipe->inResources.push_back(Resource::DummyOre);
    recipe->outResources.push_back(Resource::DummyIngot);
    recipe->duration = 5;
    database.back().available.push_back(recipe);
}