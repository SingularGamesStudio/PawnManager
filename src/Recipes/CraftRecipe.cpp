#include "CraftRecipe.h"

#include <cstring>

#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/FighterPawn.h"

template<typename T>// parses vector of simple objects inty byte array
std::vector<uint8_t> parseVector(const std::vector<T>& v) {
    unsigned int sz = v.size();
    std::vector<uint8_t> result(4 + sz * sizeof(T));
    std::memcpy(result.data(), &sz, 4);
    std::memcpy(result.data() + 4, v.data(), sz * sizeof(T));
    return result;
}

template<typename T>// parses vector of simple objects inty byte array
unsigned int unparseVector(const uint8_t* v, std::vector<T>& result) {
    unsigned int sz = 0;
    std::memcpy(&sz, v, 4);
    result.resize(sz);
    std::memcpy(result.data(), v + 4, sz * sizeof(T));
    return 4 + sz * sizeof(T);
}
#ifdef SERVER_SIDE
void CraftRecipe::finish() {
    for (ptr<Pawn> p: procPawns) { p.del(); }

    for (FighterPawnType t: outFighters) { FighterPawn::createFighterPawn(t, place.dyn_cast<Building>()).dyn_cast<Pawn>(); }
    for (Resource t: outResources) { place->addResource(t); }
    cleanup();
}
#endif

Recipe* CraftRecipe::cloneSelf() {
    CraftRecipe* res = new CraftRecipe();
    res->outResources = outResources;
    res->outFighters = outFighters;
    return res;
}