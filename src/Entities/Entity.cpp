#include "Entity.h"

#include <cstring>

#include "../Player.h"

template<typename T>
size_t copyVariable(uint8_t* dst, T src) {
    std::memcpy(dst, &src, sizeof(src));
    return sizeof(src);
}

template<typename T>
size_t initializeVariable(const uint8_t* src, T& dst) {
    std::memcpy(&dst, src, sizeof(dst));
    return sizeof(dst);
}

void Entity::changeHealth(int delta) {
    hp += delta;
    if (hp <= 0) { delete this; }
}

Entity::~Entity() {}

std::vector<uint8_t> Entity::serialize() const { return serializeSelf(); }
size_t Entity::deserialize(const std::vector<uint8_t> &data) { return deserializeSelf(data); }

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

