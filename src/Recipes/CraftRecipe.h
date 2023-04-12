#include "Recipe.h"

class CraftRecipe : public Recipe {
protected:
    Recipe* cloneSelf() override;

public:
    std::vector<FighterPawnType> outFighters;
    std::vector<Resource> outResources;

    CraftRecipe(){}

    void finish() override;


    std::vector<uint8_t> serialize() const override;
    void deserialize(const std::vector<uint8_t>& data) override;
};