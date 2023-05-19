
#include "BuildingRegisty.h"

#include "../../Recipes/BuildRecipe.h"
#include "../../Recipes/CraftRecipe.h"
#include "../../Recipes/WorkerRecipe.h"
#include "../Pawns/FighterPawn.h"
#include "../Pawns/WorkerPawn.h"
#include "Building.h"
std::vector<BuildRecipe*> BuildingRegisty::database;

void BuildingRegisty::init() {

    {
        BuildingIdea empty = BuildingIdea(Building::baseBuildingRadius, database.size());
        BuildRecipe* recEmpty = new BuildRecipe({0, 0}, empty);
        for (int i = 0; i < 1; ++i) recEmpty->inResources.push_back(Resource::Wood);
        for (int i = 0; i < 1; ++i) recEmpty->inResources.push_back(Resource::Stone);
        recEmpty->duration = 3.0;
        database.push_back(recEmpty);
    }

    {
        BuildingIdea park = BuildingIdea(Building::baseBuildingRadius * 1.25, database.size());

        CraftRecipe* wood = makeGrindRecipe({expertisesID::Lumbering}, {Resource::Wood}, 5.0);
        park.available.push_back(wood);

        CraftRecipe* fast_wood = makeGrindRecipe({expertisesID::Lumbering, expertisesID::Lumbering},
                                                  {Resource::Wood, Resource::Wood}, 3.0);
        park.available.push_back(fast_wood);

        WorkerRecipe* train = new WorkerRecipe();
        train->reqWorkers.push_back(expertisesID::Nitwit);
        train->trainExpertises.push_back(expertisesID::Lumbering);
        train->duration = 7.0;
        park.available.push_back(train);

        BuildRecipe* createPark = new BuildRecipe({0, 0}, park);
        for (int i = 0; i < 5; ++i) createPark->inResources.push_back(Resource::Wood);
        createPark->duration = 5.0;
        createPark->reqWorkers.push_back(expertisesID::Nitwit);

        database.push_back(createPark);
    }

    {
        BuildingIdea stone_mine = BuildingIdea(Building::baseBuildingRadius * 0.9, database.size());

        CraftRecipe* stone = makeGrindRecipe({expertisesID::Mining}, {Resource::Stone}, 5.0);
        stone_mine.available.push_back(stone);

        CraftRecipe* fast_stone = makeGrindRecipe({expertisesID::Mining, expertisesID::Mining},
                                                  {Resource::Stone, Resource::Stone}, 4.0);
        stone_mine.available.push_back(fast_stone);

        WorkerRecipe* train = new WorkerRecipe();
        train->reqWorkers.push_back(expertisesID::Nitwit);
        train->trainExpertises.push_back(expertisesID::Mining);
        train->duration = 7.0;
        stone_mine.available.push_back(train);

        BuildRecipe* createMine = new BuildRecipe({0, 0}, stone_mine);
        for (int i = 0; i < 5; ++i) createMine->inResources.push_back(Resource::Stone);
        createMine->duration = 5.0;
        createMine->reqWorkers.push_back(expertisesID::Nitwit);

        database.push_back(createMine);
    }

    {
        BuildingIdea iron_mine = BuildingIdea(Building::baseBuildingRadius * 0.8, database.size());

        CraftRecipe* ore = makeGrindRecipe({expertisesID::Mining}, {Resource::Ore}, 5.0);
        iron_mine.available.push_back(ore);

        CraftRecipe* fast_ore = makeGrindRecipe({expertisesID::Mining, expertisesID::Mining},
                                                  {Resource::Ore, Resource::Ore}, 4.0);
        iron_mine.available.push_back(fast_ore);

        WorkerRecipe* train = new WorkerRecipe();
        train->reqWorkers.push_back(expertisesID::Nitwit);
        train->trainExpertises.push_back(expertisesID::Mining);
        train->duration = 7.0;
        iron_mine.available.push_back(train);

        BuildRecipe* createMine = new BuildRecipe({0, 0}, iron_mine);
        for (int i = 0; i < 2; ++i) createMine->inResources.push_back(Resource::Wood);
        for (int i = 0; i < 3; ++i) createMine->inResources.push_back(Resource::Stone);
        createMine->duration = 5.0;
        createMine->reqWorkers.push_back(expertisesID::Nitwit);

        database.push_back(createMine);
    }

    {
        BuildingIdea blacksmith = BuildingIdea(Building::baseBuildingRadius * 2, database.size());
        CraftRecipe* steel = new CraftRecipe();
        steel->inResources.push_back(Resource::Ore);
        steel->outResources.push_back(Resource::Steel);
        steel->reqWorkers.push_back(expertisesID::Smithing);
        steel->duration = 3.0;
        blacksmith.available.push_back(steel);

        CraftRecipe* efficient_steel = new CraftRecipe();
        for (int i = 0; i < 2; ++i) efficient_steel->inResources.push_back(Resource::Ore);
        for (int i = 0; i < 3; ++i) efficient_steel->outResources.push_back(Resource::Steel);
        for (int i = 0; i < 2; ++i) efficient_steel->reqWorkers.push_back(expertisesID::Smithing);
        efficient_steel->duration = 6.0;
        blacksmith.available.push_back(efficient_steel);

        CraftRecipe* weapon = new CraftRecipe();
        for (int i = 0; i < 2; ++i) weapon->inResources.push_back(Resource::Steel);
        weapon->outResources.push_back(Resource::Sword);
        weapon->reqWorkers.push_back(expertisesID::Smithing);
        weapon->duration = 2;
        blacksmith.available.push_back(weapon);

        WorkerRecipe* train = new WorkerRecipe();
        train->reqWorkers.push_back(expertisesID::Nitwit);
        train->trainExpertises.push_back(expertisesID::Smithing);
        train->duration = 7.0;
        blacksmith.available.push_back(train);

        BuildRecipe* createBlacksmith = new BuildRecipe({0, 0}, blacksmith);
        for (int i = 0; i < 2; ++i) createBlacksmith->inResources.push_back(Resource::Wood);
        for (int i = 0; i < 2; ++i) createBlacksmith->inResources.push_back(Resource::Stone);
        for (int i = 0; i < 2; ++i) createBlacksmith->inResources.push_back(Resource::Ore);
        createBlacksmith->duration = 7.0;
        createBlacksmith->reqWorkers.push_back(expertisesID::Mining);
        createBlacksmith->reqWorkers.push_back(expertisesID::Lumbering);

        database.push_back(createBlacksmith);
    }

    {
        BuildingIdea barracks = BuildingIdea(Building::baseBuildingRadius * 1.1, database.size());

        CraftRecipe* train_monk = new CraftRecipe();
        train_monk->inWorkers.push_back(expertisesID::Nitwit);
        train_monk->inResources.push_back(Resource::Wood);
        train_monk->outFighters.push_back(FighterPawnType::Monk);
        train_monk->duration = 5.0;
        barracks.available.push_back(train_monk);

        CraftRecipe* train_swordsman = new CraftRecipe();
        train_swordsman->inResources.push_back(Resource::Sword);
        train_swordsman->inFighters.push_back(FighterPawnType::Monk);
        train_swordsman->outFighters.push_back(FighterPawnType::Swordsman);
        train_swordsman->duration = 5.0;
        barracks.available.push_back(train_swordsman);

        BuildRecipe* createBarracks = new BuildRecipe({0, 0}, barracks);
        for (int i = 0; i < 4; ++i) createBarracks->inResources.push_back(Resource::Wood);
        for (int i = 0; i < 2; ++i) createBarracks->inResources.push_back(Resource::Stone);
        createBarracks->duration = 7.0;
        createBarracks->reqWorkers.push_back(expertisesID::Nitwit);
        database.push_back(createBarracks);
    }
#ifdef CLIENT_SIDE
    database[0]->toBuild.name = "Platform";
    database[1]->toBuild.name = "Park";
    database[2]->toBuild.name = "Stone mine";
    database[3]->toBuild.name = "Iron mine";
    database[4]->toBuild.name = "Blacksmith";
    database[5]->toBuild.name = "Barracks";
#endif
}