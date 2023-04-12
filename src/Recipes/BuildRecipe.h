#include <utility>

#include "../Entities/Buildings/BuildingRegisty.h"
#include "../Resource.h"
#include "Recipe.h"

class BuildRecipe : public Recipe {
protected:
    Recipe* cloneSelf() override;

public:
    std::pair<double, double> pos;
    BuildingIdea toBuild;

    BuildRecipe(std::pair<double, double> pos, int index) : pos(pos), toBuild(BuildingRegisty::database[index]) {
        inResources.push_back(Resource::DummyOre);//TODO:get from database
        duration = 4;
    }

    void deserialize(const std::vector<uint8_t>& data) override{};
    void finish() override;
    std::vector<uint8_t> serialize() const override{};
};