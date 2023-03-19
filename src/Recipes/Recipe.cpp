#include <vector>
#include <cassert>
#include <set>
#include "Recipe.h"
#include "../Entities/Pawns/Pawn.h"
#include "../Resource.h"
#include "../Entities/Pawns/FighterPawn.h"
#include "../Entities/Pawns/WorkerPawn.h"
#include "../Entities/Buildings/CraftBuilding.h"


Recipe Recipe::none() {
    return Recipe();
}

bool Recipe::checkRequirements(CraftBuilding* place, bool start){
    std::set<WorkerPawn*> workersInside;
    std::set<FighterPawn*> fightersInside;
    std::multiset<Resource> resourcesInside;
    std::vector<Pawn*> usedPawns;
    std::vector<WorkerPawn*> workingPawns;
    std::vector<Resource> usedResources;
    for(Resource r:place->resources){resourcesInside.insert(r);}
    for(Pawn* p:place->pawns) {
        if (WorkerPawn *worker = dynamic_cast<WorkerPawn *>(p); worker != nullptr) {
            workersInside.insert(worker);
        } else if (FighterPawn *fighter = dynamic_cast<FighterPawn *>(p); fighter != nullptr) {
            fightersInside.insert(fighter);
        } else
            assert(0);
    }
    //TODO:копипаста
    for(FighterPawnType t : inFighters) {
        bool ok = false;
        for(auto it = fightersInside.begin(); it!=fightersInside.end(); it++){
            if((*it)->getType()==t){
                usedPawns.push_back(dynamic_cast<Pawn*>(*it));
                fightersInside.erase(it);
                ok = true;
                break;
            }
        }
        if(!ok)
            return false;
    }

    for(expertisesID t : inWorkers) {//TODO:rewrite for many expertises
        bool ok = false;
        for(auto it = workersInside.begin(); it!=workersInside.end(); it++){
            if((*it)->expertises.contains(t)){
                usedPawns.push_back(dynamic_cast<Pawn*>(*it));
                workersInside.erase(it);
                ok = true;
                break;
            }
        }
        if(!ok)
            return false;
    }

    for(expertisesID t : reqWorkers) {
        bool ok = false;
        for(auto it = workersInside.begin(); it!=workersInside.end(); it++){
            if((*it)->expertises.contains(t)){
                workingPawns.push_back(*it);
                workersInside.erase(it);
                ok = true;
                break;
            }
        }
        if(!ok)
            return false;
    }

    for(Resource t : inResources) {
        bool ok = false;
        for(auto it = resourcesInside.begin(); it!=resourcesInside.end(); it++){
            if((*it)==t){
                usedResources.push_back(t);
                resourcesInside.erase(it);
                ok = true;
                break;
            }
        }
        if(!ok)
            return false;
    }

    if(start){
        place->pawns.clear();
        place->resources.clear();
        for(Resource p :resourcesInside){place->addResource(p);}
        for(WorkerPawn* p :workersInside){place->addPawn(dynamic_cast<Pawn*>(p));}
        for(FighterPawn* p :fightersInside){place->addPawn(dynamic_cast<Pawn*>(p));}

        procResources = usedResources;
        procPawns = usedPawns;
        workers = workingPawns;

        for(Pawn* p:usedPawns){p->beIngridient();}
        for(WorkerPawn* p:workingPawns){dynamic_cast<Pawn*>(p)->beIngridient();}
    }
    return true;
}

void Recipe::start(CraftBuilding* place){
    this->place = place;
    progress = 0;
    checkRequirements(place, true);
}

void Recipe::tick(double deltaTime) {
    progress+=deltaTime;
    if(progress>=duration)
        finish();
}

void Recipe::cleanup() {
    for(WorkerPawn* p :workers){
        place->addPawn(dynamic_cast<Pawn*>(p));
        dynamic_cast<Pawn*>(p)->stopBeingIngridient();
    }
    procPawns.clear();
    procResources.clear();
    workers.clear();
    place->current = nullptr;
    progress = 0;
}

void Recipe::cancel(){
    for(Pawn* p :procPawns){
        place->addPawn(p);
        p->stopBeingIngridient();
    }
    for(Resource p :procResources){place->addResource(p);}
    cleanup();
}

void Recipe::finish() {
    for(Pawn* p :procPawns){delete p;}

    for(FighterPawnType t : outFighters) {
        place->addPawn(FighterPawn::createFighterPawn(t, place));
    }
    for(Resource t : outResources) {
        place->addResource(t);
    }
    cleanup();
}