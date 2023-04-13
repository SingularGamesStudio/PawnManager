#include "ResourceEntity.h"

#include <utility>

#include "../Resource.h"
#include "Entity.h"

std::set<ptr<ResourceEntity>> ResourceEntity::danglingResources;

ResourceEntity::ResourceEntity(int id, Resource res, std::pair<double, double> pos) : resource(res) {
    this->id = id;
    danglingResources.insert(ptr<ResourceEntity>(id));
}

ResourceEntity::~ResourceEntity() { danglingResources.erase(ptr<ResourceEntity>(id)); }
