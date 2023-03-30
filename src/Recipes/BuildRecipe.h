#include "Recipe.h"
#include "../Entities/Buildings/BuildingRegisty.h"
#include <utility>

class BuildRecipe:public Recipe {
    std::pair<double, double> pos;
    BuildingIdea toBuild;
};