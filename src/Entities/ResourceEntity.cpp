#include "ResourceEntity.h"

#include <utility>

#include "../Resource.h"
#include "Entity.h"

ResourceEntity::ResourceEntity(int id, Resource res, std::pair<double, double> pos) : resource(res) { this->id = id; }
