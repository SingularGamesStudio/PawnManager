#ifndef RECIPE_H
#define RECIPE_H
///misha does not believe in me  :(
#include <cassert>
#include <set>
#include <string>
#include <vector>

#include "../IDmanager.h"

class CraftBuilding;
class Building;
class Pawn;
class WorkerPawn;
enum class Resource : uint8_t;
enum class expertisesID : uint8_t;
enum class FighterPawnType : uint8_t;
enum class RecipeType : uint8_t { BASIC_RECIPE, CRAFT_RECIPE, BUILD_RECIPE, SHOOT_RECIPE };

class Recipe {
protected:
#ifdef SERVER_SIDE
    ptr<CraftBuilding> place;
    std::vector<ptr<WorkerPawn>> workers;
    std::vector<ptr<Pawn>> procPawns;
    std::vector<Resource> procResources;
    double progress;
#endif
    virtual Recipe* cloneSelf() = 0;

public:
#ifdef SERVER_SIDE
    bool checkRequirements(ptr<CraftBuilding> place, bool start = false);

    void start(ptr<CraftBuilding> place);

    void cleanup(ptr<Building> where = ptr<Building>());

    void cancel();

    virtual void finish() = 0;

    void tick(double deltaTime);
#endif
    Recipe* clone();

    std::vector<expertisesID> reqWorkers;

    std::vector<FighterPawnType> inFighters;
    std::vector<expertisesID> inWorkers;
    std::vector<Resource> inResources;

    double duration;
    virtual RecipeType getType() const;
    virtual std::vector<uint8_t> serialize() const;
    virtual size_t deserialize(const std::vector<uint8_t>& data);

protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const std::vector<uint8_t>& data);
};
#endif//RECIPE_H