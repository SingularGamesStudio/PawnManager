#include "Player.h"

#include <set>

#include "../Entities/Buildings/Building.h"
#include "../Entities/Pawns/FighterPawn.h"
#include "../Entities/Pawns/WorkerPawn.h"

void dfs(Building* vertex, std::multiset<Resource>& resources) {
    for (Resource r: vertex->resources) {
        if (resources.contains(r)) resources.erase(resources.find(r));
    }

    for (Building* next: vertex->children) {
        if (resources.empty()) return;
        dfs(next, resources);
    }
}

bool PendingTask::avaliable(Player* owner) {
    if (!checkPawn()) return false;
    if (task.id == TaskID::Transport) {

    } else
        return task.avaliable(owner);
}

bool Player::checkRecipe(Recipe& recipe) {
    std::multiset<Resource> resources;
    std::multiset<FighterPawnType> fighters;
    std::multiset<expertisesID> workers;
    for (Resource r: recipe.inResources) { resources.insert(r); }
    for (FighterPawnType t: recipe.inFighters) { fighters.insert(t); }
    for (expertisesID t: recipe.inWorkers) { workers.insert(t); }
    for (expertisesID t: recipe.reqWorkers) { workers.insert(t); }
    dfs(hub, resources);
    if (!resources.empty()) return false;
    for (Pawn* pawn: pawns) {
        if (WorkerPawn* worker = dynamic_cast<WorkerPawn*>(pawn); worker != nullptr) {
            for (expertisesID e: worker->expertises) {//TODO:change to mincost
                if (workers.contains(e)) {
                    workers.erase(workers.find(e));
                    break;
                }
            }
        } else if (FighterPawn* fighter = dynamic_cast<FighterPawn*>(pawn); fighter != nullptr) {
            if (fighters.contains(fighter->getType())) fighters.erase(fighters.find(fighter->getType()));
        }
    }
    if (!fighters.empty() || !workers.empty()) return false;
    return true;
}

bool Player::startRecipe(Recipe recipe, Building* where) {
    if (!checkRecipe(recipe)) return false;
    return true;
}