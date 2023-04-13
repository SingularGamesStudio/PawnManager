#include "Building.h"

#include <cassert>
#include <set>
#include <vector>

#include "../../Resource.h"
#include "../Entity.h"
#include "../Pawns/Pawn.h"

void Building::addResource(Resource resource) { resources.insert(resource); }

bool Building::removeResource(Resource resource) {
    auto x = resources.find(resource);
    if (x == resources.end()) return false;
    resources.erase(x);
    return true;
}

void Building::addPawn(ptr<Pawn> pawn) {
    if (pawns.contains(pawn)) assert(0);
    pawns.insert(pawn);
}

void Building::removePawn(ptr<Pawn> pawn) { pawns.erase(pawn); }