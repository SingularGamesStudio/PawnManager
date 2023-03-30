#include "Player.h"
#include "Entities/Buildings/Building.h"
#include "Entities/Buildings/CraftBuilding.h"
#include "Entities/Pawns/WorkerPawn.h"
#include "Recipes/Recipe.h"
#include "testSystem.h"
#include "Recipes/CraftRecipe.h"
#include "Entities/Buildings/BuildingRegisty.h"
#include <random>


CraftRecipe* recipe;
Player* player;
CraftBuilding* crafter;

Player* initTest() {

    player = new Player();
    BuildingRegisty::init();
    player->hub = new Building({80, -120}, player, 100);
    for(int i = 0; i<30; i++){
        player->hub->addResource(Resource::DummyOre);
    }
    for(int i = 0; i<5; i++){
        WorkerPawn* pawn = new WorkerPawn();
        pawn->create(player->hub);
        player->pawns.push_back(pawn);
    }
    crafter = new CraftBuilding({100, 120}, player, 100);
    player->hub->children.push_back(dynamic_cast<Building*>(crafter));
    recipe = new CraftRecipe();
    recipe->inResources.push_back(Resource::DummyOre);
    recipe->outResources.push_back(Resource::DummyIngot);
    recipe->duration = 5;
    return player;
}
std::mt19937 rnd(42);
void tick(double deltaTime) {
    if(crafter->current== nullptr)
        crafter->assignRecipe(recipe);
    for(Pawn* p:player->pawns){
        p->tick(deltaTime);
        if(p->currentTask.id==TaskID::Idle && rnd()%5000==0){
            p->assignTask(Task(TaskID::Transport, player->hub, crafter, Resource::DummyOre));
        }
    }
    crafter->tick(deltaTime);
}
