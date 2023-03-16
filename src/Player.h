#ifndef PLAYER_H
#define PLAYER_H

#include "Recipes/Recipe.h"
#include <vector>

class Building;
class Pawn;

class PendingTask {

};

class Player {
public:
    Building* hub;
    std::vector<Pawn*> pawns;
    std::vector<PendingTask> tasks;

    bool startRecipe(Recipe recipe, Building* where);

    bool checkRecipe(Recipe recipe);
};
#endif //PLAYER_H
