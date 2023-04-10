#ifndef CRAFTBUILDING_H
#define CRAFTBUILDING_H
#include <vector>

#include "../../Recipes/Recipe.h"
#include "Building.h"

class CraftBuilding : public Building {
private:
    void destroy() override;

    void stopRecipe();

public:
    CraftBuilding(std::pair<double, double> pos, Player* owner, unsigned int hp, double radius = baseBuildingRadius, Building* parent = nullptr);
    std::vector<Recipe*> recipes;
    Recipe* current;

    bool assignRecipe(Recipe* recipe);

    void tick(double deltaTime) override;
};
#endif//CRAFTBUILDING_H