#ifndef PLAYER_H
#define PLAYER_H

#include "Recipes/Recipe.h"
#include "Task.h"
#include <vector>

class Building;
class Pawn;
class Player;

struct PendingTask {
    Task task;

    virtual bool checkPawn() = 0;

    virtual bool execute()=0;

    bool avaliable(Player* owner) {
        if(!checkPawn())
            return false;
        if(task.id==TaskID::Transport){

        } else return task.avaliable(owner);
    }
};

struct FighterPendingTask:public PendingTask {
    FighterPawnType pawnType;

    FighterPendingTask(Task t, FighterPawnType type) : pawnType(type) {
        task = t;
    }

    bool execute() override {

    }
};


class Player {
public:
    Building* hub;
    std::vector<Pawn*> pawns;
    std::vector<PendingTask> tasks;

    bool startRecipe(Recipe recipe, Building* where);

    bool checkRecipe(Recipe& recipe);
};
#endif //PLAYER_H
