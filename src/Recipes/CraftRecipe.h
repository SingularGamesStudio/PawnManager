#include "Recipe.h"

class CraftRecipe: public Recipe {
public:
    std::vector<FighterPawnType> outFighters;
    std::vector<Resource> outResources;

    void finish() override;

    std:string serialize() const override {
    	std::string result;
    	
    	return result;
    }
    void deserialize() override {

    }
};

void CraftRecipe::finish() {
    for(Pawn* p :procPawns){delete p;}

    for(FighterPawnType t : outFighters) {
        place->addPawn(FighterPawn::createFighterPawn(t, place));
    }
    for(Resource t : outResources) {
        place->addResource(t);
    }
    cleanup();
}