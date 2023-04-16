#include "Building.h"

#include <cassert>
#include <random>
#include <set>
#include <vector>

#include "../../Player.h"
#include "../../Resource.h"
#include "../../godobject.h"
#include "../Entity.h"
#include "../Pawns/Pawn.h"
#include "../ResourceEntity.h"

void Building::addResource(Resource resource) {
    resources.insert(resource);
#ifdef SERVER_SIDE
    godObject::global_server->sendPacketAll(Event(Event::Type::BUILDING_ADD_RES, id, resource).getPacket());
#endif
}

Building::Building(int id, std::pair<double, double> pos, ptr<Player> owner, double hp, double radius, ptr<Building> parent)
    : Entity(pos, owner, hp, radius) {
    this->parent = parent;
    this->id = id;
#ifdef SERVER_SIDE
    //godObject::global_server->sendPacketAll(Event(Event::Type::BUILDING_APPEAR, id).getPacket());
#endif
}

bool Building::removeResource(Resource resource) {
    auto x = resources.find(resource);
    if (x == resources.end()) return false;
    resources.erase(x);
#ifdef SERVER_SIDE
    godObject::global_server->sendPacketAll(Event(Event::Type::BUILDING_REMOVE_RES, id, resource).getPacket());
#endif
    return true;
}

void Building::addPawn(ptr<Pawn> pawn) {
    if (pawns.contains(pawn)) assert(0);
    pawns.insert(pawn);
}

void Building::removePawn(ptr<Pawn> pawn) { pawns.erase(pawn); }

Building::~Building() {
#ifdef SERVER_SIDE
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
        auto send = makeptr<ResourceEntity>(r, pos);
        godObject::global_server->sendPacketAll(Event(Event::Type::RESOURCE_ENTITY_APPEAR, send.id).getPacket());
    }
    godObject::global_server->sendPacketAll(Event(Event::Type::BUILDING_DISAPPEAR, id).getPacket());
#endif
}

BuildingType Building::getType() const { return BuildingType::BASE_BUILDING; }

std::vector<uint8_t> Building::serialize() const { return serializeSelf(); }
size_t Building::deserialize(const uint8_t* data) { return deserializeSelf(data); }

std::vector<uint8_t> Building::serializeSelf() const {
    std::vector<uint8_t> result = Entity::serializeSelf();
    size_t size = sizeof(ptr<Building>) + sizeof(size_t) * 4 + sizeof(ptr<Building>) * children.size() +
                  sizeof(Resource) * (resources.size() + reservedResources.size()) + sizeof(ptr<Pawn>) * pawns.size();
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

size_t Building::deserializeSelf(const uint8_t* data) {
    size_t shift = Entity::deserializeSelf(data);
    const uint8_t* curr = data + shift;
    curr += initializeVariable(curr, parent);
    curr += initializeSet(curr, children);
    curr += initializeSet(curr, resources);
    curr += initializeSet(curr, reservedResources);
    curr += initializeSet(curr, pawns);
    return curr - data;
}