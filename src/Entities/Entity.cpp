#include "Entity.h"

#include <cstring>

#include "../Player.h"


#ifdef SERVER_SIDE
void Entity::changeHealth(double delta) {
    hp += delta;
    if (hp <= 0) {
        if (owner->hub.id == id) {
            owner.del();
        } else
            delete this;
    }
}
#endif

std::vector<uint8_t> Entity::serialize() const { return serializeSelf(); }
size_t Entity::deserialize(const std::vector<uint8_t>& data) { return deserializeSelf(data); }

std::vector<uint8_t> Entity::serializeSelf() const {
    std::vector<uint8_t> result = RequiresID::serializeSelf();
    size_t size = sizeof(double) * 2 + sizeof(ptr<Player>) + sizeof(std::pair<double, double>);
    result.resize(result.size() + size);
    uint8_t* curr = result.data() + result.size() - size;
    curr += copyVariable(curr, hp);
    curr += copyVariable(curr, owner);
    curr += copyVariable(curr, position);
    curr += copyVariable(curr, radius);
    return result;
}

size_t Entity::deserializeSelf(const std::vector<uint8_t>& data) {
    size_t shift = RequiresID::deserializeSelf(data);
    const uint8_t* curr = data.data() + shift;
    curr += initializeVariable(curr, hp);
    curr += initializeVariable(curr, owner);
    curr += initializeVariable(curr, position);
    curr += initializeVariable(curr, radius);
    return curr - data.data();
}
#ifdef CLIENT_SIDE
double getTime() { return clock() / CLOCKS_PER_SEC; }
void Entity::startMoveToPos(std::pair<double, double> pos, double time) {
    prevPos = getInterpolatedPos();
    this->position = pos;
    posReachTime = time;
    beginTime = getTime();
}
std::pair<double, double> Entity::getInterpolatedPos() {
    double progress = std::min((getTime() - beginTime) / posReachTime, 1.0);
    return {position.first * progress + prevPos.first * (1 - progress), position.second * progress - prevPos.second * (1 - progress)};
}

#endif
