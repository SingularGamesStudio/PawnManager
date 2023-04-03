#ifndef RECIPE_H
#define RECIPE_H
///misha does not believe in me  :(
#include <vector>
#include <cassert>
#include <string>
#include <set>
#include "../IDmanager.h"

class CraftBuilding;
class Building;
class Pawn;
class WorkerPawn;
enum class Resource;
enum class expertisesID;
enum class FighterPawnType;
enum class RecipeType : uint8_t { BASIC_RECIPE, CRAFT_RECIPE, BUILD_RECIPE, SHOOT_RECIPE };

class Recipe {
protected:
    ptr<CraftBuilding> place;
    std::vector<ptr<WorkerPawn>> workers;
    std::vector<ptr<Pawn>> procPawns;
    std::vector<Resource> procResources;
    double progress;
public:

    bool checkRequirements(ptr<CraftBuilding> place, bool start = false);

    void start(ptr<CraftBuilding> place);

    void cleanup(ptr<Building> where = ptr<Building>());

    void cancel();

    virtual void finish() = 0;

    void tick(double deltaTime);

    virtual std::vector<uint8_t> serialize() const = 0;

    virtual void deserialize(const std::vector<uint8_t>& data) = 0;

    std::vector<expertisesID> reqWorkers;

    std::vector<FighterPawnType> inFighters;
    std::vector<expertisesID> inWorkers;
    std::vector<Resource> inResources;

    double duration;

};
#endif //RECIPE_H