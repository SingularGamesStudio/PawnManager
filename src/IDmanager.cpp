#include <stdexcept>
#include "Entities/Entity.h"
#include "Entities/Buildings/Building.h"
#include "Entities/Pawns/Pawn.h"
std::unordered_map<int, Entity*> IDmanager::all;
int IDmanager::nextID;

int IDmanager::newEntity(Entity* entity) {
    all[nextID] = entity;
    return nextID++;
}

int IDmanager::newID() {
    return nextID++;
}

Pawn* IDmanager::getPawn(int id) {
    Entity* res = all[id];
    Pawn* p = dynamic_cast<Pawn*>(res);
    if(p != nullptr)
        return p;
    return nullptr;
}

Building* IDmanager::getBuilding(int id) {
    Entity* res = all[id];
    Building* p = dynamic_cast<Building*>(res);
    if(p != nullptr)
        return p;
    return nullptr;
}