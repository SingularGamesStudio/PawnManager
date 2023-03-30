#include "CraftRecipe.h"
#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/FighterPawn.h"

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