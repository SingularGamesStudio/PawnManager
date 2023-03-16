#ifndef RECIPE_H
#define RECIPE_H
///misha does not believe in me  :(
#include <vector>
#include <cassert>
#include<set>

class Building;
class Pawn;
class WorkerPawn;
enum class Resource;
enum class expertisesID;
enum class FighterPawnType;

class Recipe {
private:
    Building* place;
    std::vector<WorkerPawn*> workers;
    std::vector<Pawn*> procPawns;
    std::vector<Resource> procResources;
public:
    static Recipe none();

    bool checkRequirements(Building* place, bool start = false);

    void start(Building* place);

    void cleanup();

    void cancel();

    void finish();

    std::vector<expertisesID> reqWorkers;

    std::vector<FighterPawnType> inFighters;
    std::vector<expertisesID> inWorkers;
    std::vector<Resource> inResources;

    std::vector<FighterPawnType> outFighters;
    std::vector<Resource> outResources;

    float duration;
};
#endif //RECIPE_H