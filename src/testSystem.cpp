#include "testSystem.h"

#include <random>

#include "Entities/Buildings/Building.h"
#include "Entities/Buildings/BuildingRegisty.h"
#include "Entities/Buildings/CraftBuilding.h"
#include "Entities/Pawns/FighterPawn.h"
#include "Entities/Pawns/WorkerPawn.h"
#include "Player.h"
#include "Recipes/CraftRecipe.h"
#include "Recipes/Recipe.h"


ptr<Player> player;

ptr<Player> initTest() {
    CraftRecipe* recipe;
    ptr<CraftBuilding> crafter;
    player = makeptr<Player>();
    player->manager.owner = player;
    BuildingRegisty::init();
    player->hub = makeptr<Building>(std::make_pair(80, -120), player, 100);
    for (int i = 0; i < 30; i++) { player->hub->addResource(Resource::DummyOre); }
    for (int i = 0; i < 5; i++) {
        ptr<WorkerPawn> pawn = makeptr<WorkerPawn>();
        pawn->create(player->hub);
        pawn->expertises.insert(expertisesID::DummySmeltery);
        player->pawns.insert(pawn.dyn_cast<Pawn>());
    }
    crafter = makeptr<CraftBuilding>(std::make_pair(100, 120), player, 100);
    player->hub->children.insert(crafter.dyn_cast<Building>());
    crafter->parent = player->hub;
    recipe = new CraftRecipe();
    recipe->inResources.push_back(Resource::DummyOre);
    recipe->reqWorkers.push_back(expertisesID::DummySmeltery);
    recipe->outResources.push_back(Resource::DummyIngot);
    recipe->duration = 5;
    crafter->recipes.push_back(recipe);
    recipe = new CraftRecipe();
    recipe->inResources.push_back(Resource::DummyOre);
    recipe->outFighters.push_back(FighterPawnType::DummySwordsman);
    recipe->duration = 2;
    crafter->recipes.push_back(recipe);
    return player;
}

void tickBuildings(ptr<Building> place, double deltaTime) {
    place->tick(deltaTime);
    for (ptr<Building> ch: place->children) { tickBuildings(ch, deltaTime); }
}
std::mt19937 rnd(42);
void tick(double deltaTime) {
    player->tick();
    for (ptr<Pawn> p: player->pawns) { p->tick(deltaTime); }
    tickBuildings(player->hub, deltaTime);
}
