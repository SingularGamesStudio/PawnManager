//#include "Pawn.h"
//#include "Task.h"
#include "Recipe.h"
#include <vector>

class Building;
class Pawn;

class Player {
public:
    Building* hub;
    std::vector<Pawn*> pawns;
//    std::vector<>

    void startRecipe(Recipe recipe, Building* where);
};
