#ifndef RESOURCEENTITY_H
#define RESOURCEENTITY_H
///misha does not believe in me  :(
#include <set>
#include <utility>

#include "../Resource.h"
#include "Entity.h"

class ResourceEntity : public Entity {
public:
    Resource resource;
    ResourceEntity(int id, Resource res, std::pair<double, double> pos);
    virtual ~ResourceEntity();
    virtual std::vector<uint8_t> serialize() const;
    virtual size_t deserialize(const uint8_t* data);
#ifdef CLIENT_SIDE
    ResourceEntity() {}
#endif
protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const uint8_t* data);
};


#endif//RESOURCEENTITY_H