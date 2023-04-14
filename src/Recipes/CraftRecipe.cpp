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

std::vector<uint8_t> CraftRecipe::serialize() const {
    std::vector<uint8_t> result(1);
    result[0] = (uint8_t) RecipeType::CRAFT_RECIPE;
    std::vector<uint8_t> tmp = parseVector(reqWorkers);
    std::copy(tmp.begin(), tmp.end(), std::back_inserter(result));

    tmp = parseVector(inFighters);
    std::copy(tmp.begin(), tmp.end(), std::back_inserter(result));
    tmp = parseVector(inWorkers);
    std::copy(tmp.begin(), tmp.end(), std::back_inserter(result));
    tmp = parseVector(inResources);
    std::copy(tmp.begin(), tmp.end(), std::back_inserter(result));

    tmp = parseVector(outFighters);
    std::copy(tmp.begin(), tmp.end(), std::back_inserter(result));
    tmp = parseVector(outResources);
    std::copy(tmp.begin(), tmp.end(), std::back_inserter(result));
    return result;
}

void CraftRecipe::deserialize(const std::vector<uint8_t>& data) {
    unsigned int pos = 0;
    const uint8_t* ptr = data.data();
    pos += unparseVector(ptr + pos, reqWorkers);

    pos += unparseVector(ptr + pos, inFighters);
    pos += unparseVector(ptr + pos, inWorkers);
    pos += unparseVector(ptr + pos, inResources);

    pos += unparseVector(ptr + pos, outFighters);
    pos += unparseVector(ptr + pos, outResources);
}

Recipe* CraftRecipe::cloneSelf() {
    CraftRecipe* res = new CraftRecipe();
    res->outResources = outResources;
    res->outFighters = outFighters;
    return res;
}