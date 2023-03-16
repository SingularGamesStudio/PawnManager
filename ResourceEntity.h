#include "Resource.h"
#include "Entity.h"
#include <utility>

class ResourceEntity:public Entity {
public:
    Resource resource;
    ResourceEntity(Resource res, std::pair<double,double> pos) ;
    void destroy() override;
};