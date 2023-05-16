#include <stdexcept>

#include "../Entities/Entity.h"

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
    std::cerr << "id set is " << id << " and type is " << typeid(data).name() << std::endl;
}

std::vector<uint8_t> RequiresID::serialize() const { return serializeSelf(); }
size_t RequiresID::deserialize(const uint8_t* data) { return deserializeSelf(data); }

std::vector<uint8_t> RequiresID::serializeSelf() const {
    size_t size = sizeof(int);
    std::vector<uint8_t> result(size);
    uint8_t* curr = result.data();
    curr += copyVariable(curr, id);
    return result;
}

size_t RequiresID::deserializeSelf(const uint8_t* data) {
    const uint8_t* curr = data;
    curr += initializeVariable(curr, id);
    return curr - data;
}