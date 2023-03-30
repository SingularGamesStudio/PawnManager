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

struct PawnFinder {
    virtual Pawn* find(Player* owner) = 0;
};

struct FighterFinder:public PawnFinder {
    FighterPawnType type;
    Pawn * find(Player *owner) override ;
};

struct WorkerFinder:public PawnFinder {
    expertisesID expertise;
    Pawn * find(Player *owner) override;
};

struct PendingTask {
    Task task;

    bool checkPawn();

    bool execute();

    bool avaliable(Player* owner) {
        if(!checkPawn())
            return false;
        if(task.id==TaskID::Transport){

        } else return task.avaliable(owner);
    }
};

struct RecipeInWork {
    std::deque<std::set<PendingTask*>> steps;

    int ID;
    int priority;
    Recipe* recipe;
    Building* place;

    RecipeInWork(Recipe* recipe, Building* place, int priority);
};


class Player {
public:
    Player(){}
    Building* hub;
    std::vector<Pawn*> pawns;
    std::vector<RecipeInWork*> work;

    bool startRecipe(Recipe* recipe, Building* where);

    bool checkRecipe(Recipe* recipe);
};
#endif //PLAYER_H
