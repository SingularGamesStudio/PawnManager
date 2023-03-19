#include "../Entity.h"
#include "../../Resource.h"
#include "../Pawns/Pawn.h"
#include "Building.h"
#include <vector>
#include<set>

void Building::destroy() {}

void Building::addResource(Resource resource) {
    resources.insert(resource);
}

bool Building::removeResource(Resource resource) {
    auto x = resources.find(resource);
    if (x == resources.end())
        return false;
    resources.erase(x);
    return true;
}

void Building::addPawn(Pawn* pawn) {
    pawns.insert(pawn);
}

void Building::removePawn(Pawn* pawn) {
    pawns.erase(pawn);
}