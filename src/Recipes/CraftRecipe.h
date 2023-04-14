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

};