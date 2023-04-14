#ifndef CRAFTBUILDING_H
#define CRAFTBUILDING_H
///misha does not believe in me  :(
#include <iostream>
#include <vector>

#include "../../Recipes/Recipe.h"
#include "Building.h"

class CraftBuilding : public Building {
#ifdef SERVER_SIDE
    void stopRecipe();
#endif
public:
    CraftBuilding(int id, std::pair<double, double> pos, ptr<Player> owner, double hp, double radius = baseBuildingRadius,
                  ptr<Building> parent = ptr<Building>())
        : Building(id, pos, owner, hp, radius, parent),
          Entity(pos, owner, hp, radius),
          current(nullptr) {}
#ifdef CLIENT_SIDE
    CraftBuilding() {}
#endif
    std::vector<Recipe*> recipes;
    Recipe* current;
#ifdef SERVER_SIDE
    bool assignRecipe(Recipe* recipe);

    void tick(double deltaTime) override;
#endif
    virtual ~CraftBuilding();
    BuildingType getType() const override;
    std::vector<uint8_t> serialize() const override;
    size_t deserialize(const std::vector<uint8_t>& data) override;

protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const std::vector<uint8_t>& data);
};
#endif//CRAFTBUILDING_H