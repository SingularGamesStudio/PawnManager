#include <vector>
#include <cassert>
#include "Pawn.h"
#include "Resource.h"
#include "FighterPawn.h"
#include "WorkerPawn.h"


class Recipe {
private:
    Building* place;
    std::vector<WorkerPawn*> workers;
    std::vector<Pawn*> procPawns;
    std::vector<Resource> procResources;
public:
    static Recipe none() {
        return Recipe();
    }

    bool checkRequirements(Building* place, bool start = false){
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
            for(Resource p :resourcesInside){place->resources.push_back(p);}
            for(WorkerPawn* p :workersInside){place->pawns.push_back(dynamic_cast<Pawn*>(p));}
            for(FighterPawn* p :fightersInside){place->pawns.push_back(dynamic_cast<Pawn*>(p));}

            procResources = usedResources;
            procPawns = usedPawns;
            workers = workingPawns;

            for(Pawn* p:usedPawns){p->beIngridient();}
            for(WorkerPawn* p:workingPawns){dynamic_cast<Pawn*>(p)->beIngridient();}
            //TODO:do something with resources?
        }
        return true;
    }

    void start(Building* place){
        this->place = place;
        checkRequirements(place, true);
    }

    void cleanup() {
        for(WorkerPawn* p :workers){
            place->addPawn(dynamic_cast<Pawn*>(p));
            dynamic_cast<Pawn*>(p)->stopBeingIngridient();
        }
        procPawns.clear();
        procResources.clear();
        workers.clear();
        place = nullptr;
    }

    void cancel(){
        for(Pawn* p :procPawns){
            place->addPawn(p);
            p->stopBeingIngridient();
        }
        for(Resource p :procResources){place->addResource(p);}
        cleanup();
    }

    void finish() {
        for(Pawn* p :procPawns){delete p;}

        for(FighterPawnType t : outFighters) {
            place->addPawn(FighterPawn.createFighterPawn(t));
        }
        for(Resource t : outResources) {
            place->addResource(t);
        }
        cleanup();
    }

    std::vector<expertisesID> reqWorkers;

    std::vector<FighterPawnType> inFighters;
    std::vector<expertisesID> inWorkers;
    std::vector<Resource> inResources;

    std::vector<FighterPawnType> outFighters;
    std::vector<Resource> outResources;

    float duration;
};