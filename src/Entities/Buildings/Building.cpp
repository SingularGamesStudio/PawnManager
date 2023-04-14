#include "Building.h"

#include <cassert>
#include <random>
#include <set>
#include <vector>

#include "../../Player.h"
#include "../../Resource.h"
#include "../Entity.h"
#include "../Pawns/Pawn.h"
#include "../ResourceEntity.h"

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

Building::~Building() {
#ifdef SERVER_SIDE
    if (owner->hub.id == id) {
        std::cerr << "YOU DIED" << std::endl;
        exit(0);
    }
    while (!children.empty()) { (*children.begin()).del(); }
    if (parent) parent->children.erase(ptr<Building>(id));
    while (!pawns.empty()) (*pawns.begin()).del();
    for (Resource r: reservedResources) { resources.insert(r); }
    std::default_random_engine rng;
    std::uniform_real_distribution<float> dist(-1, 1);
    std::uniform_real_distribution<float> dist2(0, std::numbers::pi_v<float>);
    for (Resource r: resources) {
        float x = 2;
        float y = 2;
        while (x * x + y * y >= 1) {
            x = dist(rng);
            y = dist(rng);
        }
        std::pair<double, double> pos = position;
        pos.first += x * radius;
        pos.second += x * radius;
        makeptr<ResourceEntity>(r, pos);
    }
#endif
}

BuildingType Building::getType() const {
    return BuildingType::BASE_BUILDING;
}

std::vector<uint8_t> Building::serialize() const { return serializeSelf(); }
size_t Building::deserialize(const std::vector<uint8_t>& data) { return deserializeSelf(data); }

std::vector<uint8_t> Building::serializeSelf() const {
    std::vector<uint8_t> result = Entity::serializeSelf();
    size_t size = sizeof(ptr<Building>) + sizeof(size_t) * 4
            + sizeof(ptr<Building>) * children.size()
            + sizeof(Resource) * (resources.size() + reservedResources.size())
            + sizeof(ptr<Pawn>) * pawns.size();
    result.resize(result.size() + size);
    uint8_t* curr = result.data() + result.size() - size;
    curr += copyVariable(curr, parent);
    curr += copySet(curr, children);
    curr += copySet(curr, resources);
    curr += copySet(curr, reservedResources);
    curr += copySet(curr, pawns);
    result.insert(result.begin(), static_cast<uint8_t>(getType()));
    return result;
}

size_t Building::deserializeSelf(const std::vector<uint8_t>& data) {
    size_t shift = Entity::deserializeSelf(data);
    const uint8_t* curr = data.data() + shift;
    curr += initializeVariable(curr, parent);
    curr += initializeSet(curr, children);
    curr += initializeSet(curr, resources);
    curr += initializeSet(curr, reservedResources);
    curr += initializeSet(curr, pawns);
    return curr - data.data();
}