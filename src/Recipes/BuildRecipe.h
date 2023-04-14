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

    BuildRecipe(std::pair<double, double> pos, BuildingIdea toBuild) : pos(pos), toBuild(toBuild) {}

    void deserialize(const std::vector<uint8_t>& data) override{};
#ifdef SERVER_SIDE
    void finish() override;
#endif
    std::vector<uint8_t> serialize() const override{};
};