
#include "BuildingRegisty.h"

#include "../../Recipes/BuildRecipe.h"
#include "../../Recipes/CraftRecipe.h"
#include "../Pawns/FighterPawn.h"
#include "../Pawns/WorkerPawn.h"
#include "Building.h"
std::vector<BuildRecipe*> BuildingRegisty::database;

void BuildingRegisty::init() {

    {
        BuildingIdea empty = BuildingIdea(Building::baseBuildingRadius, database.size());
        BuildRecipe* recEmpty = new BuildRecipe({0, 0}, empty);
        recEmpty->inResources.push_back(Resource::DummyOre);
        recEmpty->duration = 3;
        database.push_back(recEmpty);
    }

    {
        BuildingIdea factory = BuildingIdea(Building::baseBuildingRadius * 2, database.size());
        CraftRecipe* ingot = new CraftRecipe();
        ingot->inResources.push_back(Resource::DummyOre);
        ingot->outResources.push_back(Resource::DummyIngot);
        ingot->duration = 1;
        factory.available.push_back(ingot);

        CraftRecipe* weapon = new CraftRecipe();
        weapon->inResources.push_back(Resource::DummyIngot);
        weapon->outResources.push_back(Resource::DummyWeapon);
        weapon->reqWorkers.push_back(expertisesID::DummySmeltery);
        weapon->duration = 2;
        factory.available.push_back(weapon);

        CraftRecipe* train = new CraftRecipe();
        train->inResources.push_back(Resource::DummyWeapon);
        train->inWorkers.push_back(expertisesID::DummySmeltery);
        train->outFighters.push_back(FighterPawnType::DummySwordsman);
        weapon->duration = 2;
        factory.available.push_back(train);

        BuildRecipe* recFactory = new BuildRecipe({0, 0}, factory);
        recFactory->inResources.push_back(Resource::DummyOre);
        recFactory->inResources.push_back(Resource::DummyOre);
        recFactory->inResources.push_back(Resource::DummyOre);
        recFactory->duration = 3;
        recFactory->reqWorkers.push_back(expertisesID::DummySmeltery);
        recFactory->reqWorkers.push_back(expertisesID::DummySmeltery);

        database.push_back(recFactory);
    }
}