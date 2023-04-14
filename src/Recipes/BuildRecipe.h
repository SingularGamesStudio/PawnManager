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
    BuildRecipe() : toBuild(BuildingRegisty::database[0]->toBuild) {}

    BuildRecipe(std::pair<double, double> pos, BuildingIdea toBuild) : pos(pos), toBuild(toBuild) {}

#ifdef SERVER_SIDE
    void finish() override;
#endif
    RecipeType getType() const override;
    std::vector<uint8_t> serialize() const override;
    size_t deserialize(const std::vector<uint8_t>& data) override;

protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const std::vector<uint8_t>& data);
};