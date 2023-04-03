#include "Recipe.h"

class CraftRecipe: public Recipe {
public:
    std::vector<FighterPawnType> outFighters;
    std::vector<Resource> outResources;

    void finish() override;

    std::vector<uint8_t> serialize() const override;
    void deserialize(const std::vector<uint8_t>& data) override;
};