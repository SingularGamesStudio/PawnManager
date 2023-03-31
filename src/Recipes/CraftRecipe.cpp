#include "CraftRecipe.h"
#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/FighterPawn.h"

template<typename T> // parses vector of simple objects inty byte array
std::vector<uint8_t> parseVector(const std::vector<T>& v) {
    unsigned int sz = v.size();
    std::vector<uint8_t> result(4 + sz * sizeof(T));
    std::copy(result.data(), &sz, 4);
    std::copy(result.data() + 4, v.data(), sz * sizeof(T));
}

template<typename T> // parses vector of simple objects inty byte array
unsigned int unparseVector(const uint8_t* v, std::vector<T>& result) {
    unsigned int sz = 0;
    std::copy(&sz, v, 4);
    result.resize(sz);
    std::copy(result.data(), v + 4, sz * sizeof(T));
    return 4 + sz * sizeof(T);
}

void CraftRecipe::finish() {
    for(Pawn* p : procPawns) {delete p;}

    for(FighterPawnType t : outFighters) {
        place->addPawn(FighterPawn::createFighterPawn(t, place));
    }
    for(Resource t : outResources) {
        place->addResource(t);
    }
    cleanup();
}

std::vector<uint8_t> CraftRecipe::serialize() const {
    std::vector<uint8_t> result(1);
    result[0] = (uint8_t)RecipeType::CRAFT_RECIPE;
    result += parseVector(reqWorkers);

    result += parseVector(inFighters);
    result += parseVector(inWorkers);
    result += parseVector(inResources);

    result += parseVector(outFighters);
    result += parseVector(outResources);
    return result;
}

void CraftRecipe::deserialize(const std::vector<uint8_t>& data) {
    unsigned int pos = 0;
    const uint8_t* ptr = data.data();
    pos += parseVector(ptr + pos, reqWorkers);

    pos += parseVector(ptr + pos, inFighters);
    pos += parseVector(ptr + pos, inWorkers);
    pos += parseVector(ptr + pos, inResources);

    pos += parseVector(ptr + pos, outFighters);
    pos += parseVector(ptr + pos, outResources);
}