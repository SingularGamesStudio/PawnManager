#include "Recipe.h"
#include <utility>

class BuildRecipe: public Recipe {
    std::pair<double, double> pos;
    double r;
    BuildingIdea toBuild;
};