#include "Recipe.h"

#include <cassert>
#include <set>
#include <vector>

#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/FighterPawn.h"
#include "../Entities/Pawns/Pawn.h"
#include "../Entities/Pawns/WorkerPawn.h"
#include "../Resource.h"
#include "../testSystem.h"

bool Recipe::checkRequirements(ptr<CraftBuilding> place, bool start) {
    std::set<ptr<WorkerPawn>> workersInside;
    std::set<ptr<FighterPawn>> fightersInside;
    std::multiset<Resource> resourcesInside;
    std::vector<ptr<Pawn>> usedPawns;
    std::vector<ptr<WorkerPawn>> workingPawns;
    std::vector<Resource> usedResources;
    for (Resource r: place->resources) { resourcesInside.insert(r); }
    for (ptr<Pawn> p: place->pawns) {
        if (ptr<WorkerPawn> worker = static_cast<ptr<WorkerPawn>>(p); worker) {
            workersInside.insert(worker);
        } else if (ptr<FighterPawn> fighter = static_cast<ptr<FighterPawn>>(p); fighter) {
            fightersInside.insert(fighter);
        } else
            assert(0);
    }
    // TODO:копипаста
    for (FighterPawnType t: inFighters) {
        bool ok = false;
        for (auto it = fightersInside.begin(); it != fightersInside.end(); it++) {
            if ((*it)->getType() == t) {
                usedPawns.push_back(static_cast<ptr<Pawn>>(*it));
                fightersInside.erase(it);
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }

    for (expertisesID t: inWorkers) {// TODO:rewrite for many expertises
        bool ok = false;
        for (auto it = workersInside.begin(); it != workersInside.end(); it++) {
            if ((*it)->expertises.contains(t)) {
                usedPawns.push_back(static_cast<ptr<Pawn>>(*it));
                workersInside.erase(it);
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }

    for (expertisesID t: reqWorkers) {
        bool ok = false;
        for (auto it = workersInside.begin(); it != workersInside.end(); it++) {
            if ((*it)->expertises.contains(t)) {
                workingPawns.push_back(*it);
                workersInside.erase(it);
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }

    for (Resource t: inResources) {
        bool ok = false;
        for (auto it = resourcesInside.begin(); it != resourcesInside.end(); it++) {
            if ((*it) == t) {
                usedResources.push_back(t);
                resourcesInside.erase(it);
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }

    if (start) {
        place->pawns.clear();
        place->resources.clear();
        for (Resource p: resourcesInside) { place->addResource(p); }
        for (ptr<WorkerPawn> p: workersInside) { place->addPawn(static_cast<ptr<Pawn>>(p)); }
        for (ptr<FighterPawn> p: fightersInside) { place->addPawn(static_cast<ptr<Pawn>>(p)); }

        procResources = usedResources;
        procPawns = usedPawns;
        workers = workingPawns;

        // for(ptr<Pawn> p:usedPawns){p->beIngridient();}
        // for(ptr<WorkerPawn>
        // p:workingPawns){static_cast<ptr<Pawn>>(p)->beIngridient();}
    }
    return true;
}

void Recipe::start(ptr<CraftBuilding> place) {
    this->place = place;
    progress = 0;
    checkRequirements(place, true);
}

void Recipe::tick(double deltaTime) {
    progress += deltaTime;
    if (progress >= duration) finish();
}

Recipe *Recipe::clone() {
    Recipe *res = cloneSelf();
    res->progress = 0;
    res->inResources = inResources;
    res->inWorkers = inWorkers;
    res->inFighters = inFighters;
    res->duration = duration;
    res->reqWorkers = reqWorkers;
    return res;
}

void Recipe::cleanup(ptr<Building> where) {
    if (!where) {
        where = static_cast<ptr<Building>>(place);
        place->current = nullptr;
    }
    for (ptr<WorkerPawn> p: workers) {
        where->addPawn(static_cast<ptr<Pawn>>(p));
        static_cast<ptr<Pawn>>(p)->stopBeingIngridient();
    }
    procPawns.clear();
    procResources.clear();
    workers.clear();
    progress = 0;
}

void Recipe::cancel() {
    for (ptr<Pawn> p: procPawns) {
        place->addPawn(p);
        p->stopBeingIngridient();
    }
    for (Resource p: procResources) { place->addResource(p); }
    cleanup();
}