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
};