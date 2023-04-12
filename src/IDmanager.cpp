#include <stdexcept>

#include "Entities/Buildings/Building.h"
#include "Entities/Entity.h"
#include "Entities/Pawns/Pawn.h"
std::unordered_map<int, void*> IDmanager::all;
int IDmanager::nextID;

int IDmanager::newID() { return nextID++; }

int IDmanager::newObject(void* ptr) {
    all[nextID] = ptr;
    return nextID++;
}

void* IDmanager::get(int id) { return all[id]; }

void IDmanager::set(int id, void* data) { all[id] = data; }
