#ifndef RECIPE_H
#define RECIPE_H
///misha does not believe in me  :(
#include <vector>
#include <cassert>
#include<set>

class CraftBuilding;
class Pawn;
class WorkerPawn;
enum class Resource;
enum class expertisesID;
enum class FighterPawnType;

class Recipe {
private:
    CraftBuilding* place;
    std::vector<WorkerPawn*> workers;
    std::vector<Pawn*> procPawns;
    std::vector<Resource> procResources;
    float progress;
public:
    static Recipe none();

    bool checkRequirements(CraftBuilding* place, bool start = false);

    void start(CraftBuilding* place);

    void cleanup();

    void cancel();

    void finish();

    void tick();

    std::vector<expertisesID> reqWorkers;

    std::vector<FighterPawnType> inFighters;
    std::vector<expertisesID> inWorkers;
    std::vector<Resource> inResources;

    std::vector<FighterPawnType> outFighters;
    std::vector<Resource> outResources;

    float duration;

};
#endif //RECIPE_H