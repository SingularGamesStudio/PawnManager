#include <utility>

#include "../Core/Resource.h"
#include "../Entities/Buildings/BuildingRegisty.h"
#include "../Entities/Entity.h"
#include "Recipe.h"

class BuildRecipe : public Recipe {
protected:
    Recipe* cloneSelf() override;

public:
    Position pos;
    BuildingIdea toBuild;
    BuildRecipe() : toBuild(BuildingRegisty::database[0]->toBuild) {}

    BuildRecipe(Position pos, BuildingIdea toBuild) : pos(pos), toBuild(toBuild) {}

#ifdef SERVER_SIDE
    void finish() override;
#endif
    RecipeType getType() const override;
    std::vector<uint8_t> serialize() const override;
    size_t deserialize(const uint8_t* data) override;

protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const uint8_t* data);
};