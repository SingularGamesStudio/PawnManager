#include "Recipe.h"
#include <utility>

class BuildRecipe:Recipe {
    std::pair<double, double> pos;
    double r;
    BuildingIdea toBuild;
};