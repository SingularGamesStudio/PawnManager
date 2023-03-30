#ifndef PLAYER_H
#define PLAYER_H

#include "Recipes/Recipe.h"
#include "Task.h"
#include <vector>
#include <deque>

enum class expertisesID;
class Building;
class Pawn;
class Player;

struct PawnReq {
    virtual Pawn* find(Player* owner) = 0;
};

struct FighterReq:public PawnReq {
    FighterPawnType type;
    FighterReq(FighterPawnType type) : type(type){}

    Pawn * find(Player *owner) override {};
};

struct WorkerReq:public PawnReq {
    expertisesID expertise;
    WorkerReq(expertisesID expertise):expertise(expertise){}

    Pawn * find(Player *owner) override {};
};

struct PendingRecipe {
    std::multiset<Resource> needResources;
    std::multiset<Resource> movedResources;
    std::vector<PawnReq*> needPawns;
    std::vector<PawnReq*> movedPawns;

    int ID;
    int priority;
    Recipe* recipe;
    Building* place;

    PendingRecipe(Recipe* recipe, Building* place, int priority);
};


class Player {
public:
    Player(){}
    Building* hub;
    std::vector<Pawn*> pawns;
    std::set<PendingRecipe*> work;

    bool startRecipe(Recipe* recipe, Building* where);

    bool checkRecipe(Recipe* recipe);

    CraftBuilding* placeBlueprint(std::pair<double, double> pos, Building* parent, double r);

    void tick();
};
#endif //PLAYER_H
