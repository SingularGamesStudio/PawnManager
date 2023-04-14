#include <stdexcept>

#include "Entities/Entity.h"

std::unordered_map<int, RequiresID*> IDmanager::all;
int IDmanager::nextID = 0;

int IDmanager::newID() { return nextID++; }

int IDmanager::newObject(RequiresID* ptr) {
    all[nextID] = ptr;
    return nextID++;
}

RequiresID* IDmanager::get(int id) { return all[id]; }

void IDmanager::set(int id, RequiresID* data) {
    all[id] = data;
    if (id == 0 && data == nullptr) {
        throw "aboba";
        std::cout << "a";
    }
}
