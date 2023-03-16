#include "Entity.h"
#include "Resource.h"
#include "Pawn.h"
#include <vector>
#include<set>

class Building: public virtual Entity{
private:
    void destroy() override {}
public:
    Building* parent = nullptr;
    std::vector<Building*> children;
    std::multiset<Resource> resources;
    std::set<Pawn*> pawns;

    void addResource(Resource resource) {
        resources.insert(resource);
    }

    void removeResource(Resource resource) {
        resources.erase(resources.find(resource));
    }

    void addPawn(Pawn* pawn) {
        pawns.insert(pawn);
    }

    void removePawn(Pawn* pawn) {
        pawns.erase(pawn);
    }
};