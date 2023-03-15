#include "Entity.h"
#include "Resource.h"
#include "Pawn.h"
#include <vector>

class Building: virtual Entity{
private:
    void destroy() override {}
public:
    Building* parent = nullptr;
    std::vector<Building*> children;
    std::vector<Resource*> resources;
    std::vector<Pawn*> pawns;


    //TODO
    void addResource(Resource* resource) {
        resources.push_back(resource);
    }

    void addPawn(Pawn* pawn) {
        pawns.push_back(pawn);
    }
};