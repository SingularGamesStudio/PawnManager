#ifndef CRAFTBUILDING_H
#define CRAFTBUILDING_H
///misha does not believe in me  :(
#include "Building.h"
#include "../../Recipes/Recipe.h"
#include <vector>

class CraftBuilding:public Building {
private:
    void destroy() override;

    void stopRecipe();
public:
    CraftBuilding(std::pair<double, double> pos, Player* owner, unsigned int hp, double radius = baseBuildingRadius, Building* parent= nullptr):
    Building(pos, owner, hp, radius, parent) {
    }
    std::vector<Recipe*> recipes;
    Recipe* current;

    bool assignRecipe(Recipe* recipe);

    void tick() override;
};
#endif //CRAFTBUILDING_H