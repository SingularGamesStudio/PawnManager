#include "ResourceEntity.h"

#include <utility>

#include "../Resource.h"
#include "../godobject.h"
#include "Entity.h"

ResourceEntity::ResourceEntity(int id, Resource res, std::pair<double, double> pos) : Entity(pos, ptr<Player>(), 1, 1) {
    resource = res;
    this->id = id;
#ifdef CLIENT_SIDE
    godObject::local_server->danglingResources.insert(ptr<ResourceEntity>(id));
#endif
#ifdef SERVER_SIDE
    //godObject::global_server->sendPacketAll(Event(Event::Type::RESOURCE_ENTITY_APPEAR, id).getPacket());
#endif
}

ResourceEntity::~ResourceEntity() {
#ifdef CLIENT_SIDE
    godObject::local_server->danglingResources.erase(ptr<ResourceEntity>(id));
#endif
#ifdef SERVER_SIDE
    godObject::global_server->sendPacketAll(Event(Event::Type::RESOURCE_ENTITY_DISAPPEAR, id).getPacket());
#endif
}

std::vector<uint8_t> ResourceEntity::serialize() const { return serializeSelf(); }
size_t ResourceEntity::deserialize(const uint8_t* data) { return deserializeSelf(data); }

std::vector<uint8_t> ResourceEntity::serializeSelf() const {
    std::vector<uint8_t> result = Entity::serializeSelf();
    size_t size = sizeof(Resource);
    result.resize(result.size() + size);
    uint8_t* curr = result.data() + result.size();
    curr += copyVariable(curr, resource);
    return result;
}

size_t ResourceEntity::deserializeSelf(const uint8_t* data) {
    size_t shift = Entity::deserializeSelf(data);
    const uint8_t* curr = data + shift;
    curr += initializeVariable(curr, resource);
    return curr - data;
}
