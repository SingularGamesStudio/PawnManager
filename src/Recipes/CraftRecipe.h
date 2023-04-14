#include "Recipe.h"

class CraftRecipe : public Recipe {
protected:
    Recipe* cloneSelf() override;

public:
    std::vector<FighterPawnType> outFighters;
    std::vector<Resource> outResources;

    CraftRecipe() {}
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