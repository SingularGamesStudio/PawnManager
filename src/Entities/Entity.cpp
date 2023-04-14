#include "Entity.h"

#include <cstring>

#include "../Player.h"


#ifdef SERVER_SIDE
void Entity::changeHealth(double delta) {
    hp += delta;
    if (hp <= 0) { delete this; }
}
#endif

std::vector<uint8_t> Entity::serialize() const { return serializeSelf(); }
size_t Entity::deserialize(const std::vector<uint8_t>& data) { return deserializeSelf(data); }

std::vector<uint8_t> Entity::serializeSelf() const {
    size_t size = sizeof(double) * 2 + sizeof(ptr<Player>) + sizeof(std::pair<double, double>);
    std::vector<uint8_t> result(size);
    uint8_t* curr = result.data();
    curr += copyVariable(curr, hp);
    curr += copyVariable(curr, owner);
    curr += copyVariable(curr, position);
    curr += copyVariable(curr, radius);
    return result;
}

size_t Entity::deserializeSelf(const std::vector<uint8_t>& data) {
    const uint8_t* curr = data.data();
    curr += initializeVariable(curr, hp);
    curr += initializeVariable(curr, owner);
    curr += initializeVariable(curr, position);
    curr += initializeVariable(curr, radius);
    return curr - data.data();
}
