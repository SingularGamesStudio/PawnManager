#ifndef CRAFTBUILDING_H
#define CRAFTBUILDING_H
///misha does not believe in me  :(
#include "Building.h"
#include "../../Recipes/Recipe.h"
#include <vector>

class CraftBuilding:Building {
private:
    void destroy() override;

    void stopRecipe();
public:
    std::vector<Recipe> recipes;
    Recipe current;
    double progress;

    void assignRecipe(Recipe recipe);
};
#endif //CRAFTBUILDING_H