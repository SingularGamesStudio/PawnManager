#include "Entity.h"
#include "Resource.h"

class ResourceEntity:Entity {
private:
    void destroy() override {}
public:
    Resource type;
};