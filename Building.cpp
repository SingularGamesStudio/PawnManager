#include "Entity.h"
#include "Resource.h"
#include "Pawn.h"
#include <vector>
#include<set>

void Building::destroy() {}

void Building::addResource(Resource resource) {
    resources.insert(resource);
}

void Building::removeResource(Resource resource) {
    resources.erase(resources.find(resource));
}

void Building::addPawn(Pawn* pawn) {
    pawns.insert(pawn);
}

void Building::removePawn(Pawn* pawn) {
    pawns.erase(pawn);
}