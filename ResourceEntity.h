#include "Entity.h"
#include "Resource.h"

class ResourceEntity:Entity {
private:
    void destroy() override {}
public:
    Resource resource;
    ResourceEntity(Resource res, std::pair<double,double> pos) : resource(res), position(pos) {

    }
    void destroy() {
        ~ResourceEntity();
    }
};