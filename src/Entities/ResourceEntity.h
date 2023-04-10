#ifndef RESOURCEENTITY_H
#define RESOURCEENTITY_H
#include <utility>

#include "../Core/Resource.h"
#include "Entity.h"

class ResourceEntity : public Entity {
public:
    Resource resource;
    ResourceEntity(Resource res, Position pos);
    void destroy() override;
};
#endif//RESOURCEENTITY_H