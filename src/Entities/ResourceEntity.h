#ifndef RESOURCEENTITY_H
#define RESOURCEENTITY_H
///misha does not believe in me  :(
#include <set>
#include <utility>

#include "../Resource.h"
#include "Entity.h"

class ResourceEntity : public Entity {
public:
    static std::set<ptr<ResourceEntity>> danglingResources;
    Resource resource;
    ResourceEntity(int id, Resource res, std::pair<double, double> pos);
    virtual ~ResourceEntity();
};


#endif//RESOURCEENTITY_H