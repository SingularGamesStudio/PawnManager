#include "Player.h"
#include "Entities/Buildings/Building.h"
#include "Entities/Buildings/CraftBuilding.h"
#include "Entities/Pawns/WorkerPawn.h"
#include "Recipes/Recipe.h"
#include "testSystem.h"


Recipe* recipe;
Player* player;
CraftBuilding* crafter;

Player* initTest() {
    player = new Player();
    player->hub = new Building({100, 50}, player, 100);
    for(int i = 0; i<30; i++){
        player->hub->addResource(Resource::DummyOre);
    }
    for(int i = 0; i<30; i++){
        WorkerPawn* pawn = new WorkerPawn();
        pawn->create(player->hub);
        player->pawns.push_back(pawn);
        player->hub->addPawn(pawn);
    }
    crafter = new CraftBuilding({100, 120}, player, 100);
    crafter->position = {100, 120};
    crafter->radius = Building::baseBuildingRadius;
    player->hub->children.push_back(dynamic_cast<Building*>(crafter));
    crafter->parent = player->hub;
    recipe = new Recipe();
    recipe->inResources.push_back(Resource::DummyOre);
    recipe->outResources.push_back(Resource::DummyIngot);
    recipe->duration = 60;


    return player;
}

void tick() {
    if(crafter->current== nullptr)
        crafter->assignRecipe(recipe);
    for(Pawn* p:player->pawns){
        p->tick();
        if(p->currentTask.id==TaskID::Idle){
            p->assignTask(Task(TaskID::Transport, player->hub, crafter, Resource::DummyOre));
        }
    }
    crafter->tick();
}