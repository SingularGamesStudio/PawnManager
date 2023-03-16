#include "Entity.h"
#include "Resource.h"
#include "Pawn.h"
#include <vector>

class Building: public virtual Entity{
private:
    void destroy() override {}
public:
    Building* parent = nullptr;
    std::vector<Building*> children;
    std::vector<Resource*> resources;
    std::vector<Pawn*> pawns;

    void addResource(Resource* resource) {
        resources.push_back(resource);
        resource->position = this;
        owner.addResource(resource);
    }

    void addPawn(Pawn* pawn) {
        pawns.push_back(pawn);
        pawn->inside = this;
        owner.addPawn(pawn);
    }
};