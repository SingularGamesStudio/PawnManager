#ifndef RESOURCEENTITY_H
#define RESOURCEENTITY_H
///misha does not believe in me  :(
#include "Resource.h"
#include "Entity.h"
#include <utility>

class ResourceEntity:public Entity {
public:
    Resource resource;
    ResourceEntity(Resource res, std::pair<double,double> pos) ;
    void destroy() override;
};
#endif //RESOURCEENTITY_H