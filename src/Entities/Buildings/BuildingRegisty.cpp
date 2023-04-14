
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
        recEmpty->inResources.push_back(Resource::Ingot);
        recEmpty->inResources.push_back(Resource::Ingot);
        recEmpty->duration = 3;
        database.push_back(recEmpty);
    }

    {
        BuildingIdea blacksmith = BuildingIdea(Building::baseBuildingRadius * 2, database.size());
        CraftRecipe* ingot = new CraftRecipe();
        ingot->inResources.push_back(Resource::Ore);
        ingot->outResources.push_back(Resource::Ingot);
        ingot->duration = 1;
        blacksmith.available.push_back(ingot);

        CraftRecipe* weapon = new CraftRecipe();
        weapon->inResources.push_back(Resource::Ingot);
        weapon->outResources.push_back(Resource::Weapon);
        weapon->reqWorkers.push_back(expertisesID::Smeltery);
        weapon->duration = 2;
        blacksmith.available.push_back(weapon);

        BuildRecipe* createBlacksmith = new BuildRecipe({0, 0}, blacksmith);
        createBlacksmith->inResources.push_back(Resource::Ore);
        createBlacksmith->inResources.push_back(Resource::Ore);
        createBlacksmith->inResources.push_back(Resource::Ore);
        createBlacksmith->duration = 10;
        createBlacksmith->reqWorkers.push_back(expertisesID::Smeltery);
        createBlacksmith->reqWorkers.push_back(expertisesID::Smeltery);

        database.push_back(createBlacksmith);
    }

    {
        BuildingIdea mine = BuildingIdea(Building::baseBuildingRadius * 0.25, database.size());

        CraftRecipe* ore = new CraftRecipe();
        ore->duration = 2;
        ore->reqWorkers.push_back(expertisesID::Nitwit);
        ore->outResources.push_back(Resource::Ore);
        mine.available.push_back(ore);

        CraftRecipe* fastOre = new CraftRecipe();
        ore->duration = 1;
        ore->reqWorkers.push_back(expertisesID::Nitwit);
        ore->reqWorkers.push_back(expertisesID::Nitwit);
        ore->outResources.push_back(Resource::Ore);
        mine.available.push_back(ore);

        BuildRecipe* createMine = new BuildRecipe({0, 0}, mine);
        createMine->inResources.push_back(Resource::Ore);
        createMine->duration = 5;
        createMine->reqWorkers.push_back(expertisesID::Smeltery);

        database.push_back(createMine);
    }

    {
        BuildingIdea barracks = BuildingIdea(Building::baseBuildingRadius * 1.25, database.size());

        CraftRecipe* trainSwordsman = new CraftRecipe();
        trainSwordsman->inResources.push_back(Resource::Weapon);
        trainSwordsman->reqWorkers.push_back(expertisesID::Smeltery);
        trainSwordsman->inWorkers.push_back(expertisesID::Nitwit);
        trainSwordsman->outFighters.push_back(FighterPawnType::Swordsman);
        trainSwordsman->duration = 2;
        barracks.available.push_back(trainSwordsman);

        BuildRecipe* createBarracks = new BuildRecipe({0, 0}, barracks);
        createBarracks->inResources.push_back(Resource::Weapon);
        createBarracks->inResources.push_back(Resource::Weapon);
        createBarracks->inResources.push_back(Resource::Weapon);
        createBarracks->duration = 7;
        createBarracks->reqWorkers.push_back(expertisesID::Metalworking);
        database.push_back(createBarracks);
    }

    {
        BuildingIdea temple = BuildingIdea(Building::baseBuildingRadius * 3, database.size());

        CraftRecipe* prayForPower = new CraftRecipe();
        prayForPower->reqWorkers.push_back(expertisesID::Nitwit);
        prayForPower->inWorkers.push_back(expertisesID::Nitwit);
        prayForPower->outFighters.push_back(FighterPawnType::Monk);
        prayForPower->duration = 15;
        temple.available.push_back(prayForPower);

        CraftRecipe* prayForResources = new CraftRecipe();
        prayForResources->reqWorkers.push_back(expertisesID::Nitwit);
        prayForResources->reqWorkers.push_back(expertisesID::Nitwit);
        prayForResources->reqWorkers.push_back(expertisesID::Nitwit);
        prayForResources->outResources.push_back(Resource::Ore);
        prayForResources->outResources.push_back(Resource::Ingot);
        prayForResources->outResources.push_back(Resource::Weapon);
        prayForResources->duration = 15;

        BuildRecipe* createTemple = new BuildRecipe({0, 0}, temple);
        createTemple->inResources.push_back(Resource::Ingot);
        createTemple->inResources.push_back(Resource::Ingot);
        createTemple->inResources.push_back(Resource::Ingot);
        createTemple->duration = 30;
        createTemple->reqWorkers.push_back(expertisesID::Metalworking);
        createTemple->reqWorkers.push_back(expertisesID::Smeltery);
        createTemple->reqWorkers.push_back(expertisesID::Nitwit);
        database.push_back(createTemple);
    }

}