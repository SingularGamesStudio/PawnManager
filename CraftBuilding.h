#include "Building.h"
#include "Recipe.h"
#include <vector>

class CraftBuilding:Building {
private:
    void destroy() override{}

    void stopRecipe() {
        progress = 0;
        current = Recipe.none();

    }
public:
    std::vector<Recipe> recipes;
    Recipe current;
    double progress;

    void assignRecipe(Recipe recipe){
        stopRecipe();
        current = recipe;
    }
};
