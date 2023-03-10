#include "Entity.h"
#include "Resource.h"
#include <vector>

class Building: virtual Entity{
private:
    void destroy() override {}
public:
    Building* parent = nullptr;
    std::vector<Building*> children;
    std::vector<Resource> resources;
    std::vector<Pawn*> pawns;

    void addResource(Resource resource) {
        resources.push_back(resource);
    }
};