#ifndef PLAYER_H
#define PLAYER_H

#include "Recipes/Recipe.h"
#include "Task.h"
#include <vector>
#include <deque>

class Building;
class Pawn;
class Player;

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

/*struct FighterPendingTask:public PendingTask {
    FighterPawnType pawnType;

    FighterPendingTask(Task t, FighterPawnType type) : pawnType(type) {
        task = t;
    }

    bool execute() override {
        ///TO DO
        return false;
    }
};*/

struct RecipeInWork {
    std::deque<std::set<PendingTask*>> steps;

    int ID;
    int priority;
    Recipe* recipe;
    Building* place;

    RecipeInWork(Recipe* recipe, Building* place);
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
