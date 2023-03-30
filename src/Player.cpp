#include "Player.h"
#include "Entities/Buildings/Building.h"
#include "Entities/Pawns/WorkerPawn.h"
#include "Entities/Pawns/FighterPawn.h"
#include "Recipes/BuildRecipe.h"
#include "Entities/Buildings/CraftBuilding.h"
#include <set>
#include <vector>

void dfs(Building* vertex, std::multiset<Resource>& resources){
    for(Resource r:vertex->resources) {
        if(resources.contains(r))
            resources.erase(resources.find(r));
    }

    for(Building* next:vertex->children) {
        if (resources.empty())
            return;
        dfs(next, resources);
    }
}

bool Player::checkRecipe(Recipe* recipe) {
    std::multiset<Resource> resources;
    std::multiset<FighterPawnType> fighters;
    std::multiset<expertisesID> workers;
    for(Resource r:recipe->inResources){resources.insert(r);}
    for(FighterPawnType t:recipe->inFighters){fighters.insert(t);}
    for(expertisesID t:recipe->inWorkers){workers.insert(t);}
    for(expertisesID t:recipe->reqWorkers){workers.insert(t);}
    dfs(hub, resources);
    if(!resources.empty())
        return false;
    for(Pawn* pawn:pawns){
        if(WorkerPawn* worker = dynamic_cast<WorkerPawn*>(pawn); worker!=nullptr) {
            for(expertisesID e:worker->expertises){//TODO:change to mincost
                if(workers.contains(e)){
                    workers.erase(workers.find(e));
                    break;
                }
            }
        } else if(FighterPawn* fighter = dynamic_cast<FighterPawn*>(pawn); fighter!=nullptr) {
            if(fighters.contains(fighter->getType()))
                fighters.erase(fighters.find(fighter->getType()));
        }
    }
    if(!fighters.empty() || !workers.empty())
        return false;
    return true;
}

CraftBuilding* Player::placeBlueprint(std::pair<double, double> pos, Building *parent, double r) {
    CraftBuilding* blueprint = new CraftBuilding(pos, this, 100, r, parent);
    parent->children.push_back(dynamic_cast<Building*>(blueprint));
    return blueprint;
}

bool Player::startRecipe(Recipe* recipe, Building* where) {
    if(!checkRecipe(recipe))
        return false;
    if(dynamic_cast<BuildRecipe*>(recipe)!= nullptr) {
        BuildRecipe* brecipe = dynamic_cast<BuildRecipe*>(recipe);
        where = placeBlueprint(brecipe->pos, where, brecipe->toBuild.radius);
    }
    work.push_back(new RecipeInWork(recipe, where, 0));
    return true;
}



RecipeInWork::RecipeInWork(Recipe *recipe, Building *place, int priority): recipe(recipe), place(place), priority(priority) {
    steps.push_back(std::set<PendingTask*>());
    for(Resource r :recipe->inResources) {

    }
}

