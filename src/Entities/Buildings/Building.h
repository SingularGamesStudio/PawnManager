#ifndef BUILDING_H
#define BUILDING_H
///misha does not believe in me  :(
#include "../../Resource.h"
#include "../Entity.h"
#include <vector>
#include<set>

class Pawn;

class Building: public virtual Entity{
private:
    void destroy() override;
public:
    constexpr static const double baseBuildingRadius = 30;
    Building(std::pair<double, double> pos, Player* owner, unsigned int hp, double radius = baseBuildingRadius, Building* parent= nullptr):
        Entity(pos, owner, hp, radius) {
        this->parent = parent;
    }
    Building* parent = nullptr;
    std::vector<Building*> children;
    std::multiset<Resource> resources;
    std::set<Pawn*> pawns;

    void addResource(Resource resource);

    void removeResource(Resource resource);

    void addPawn(Pawn* pawn);

    void removePawn(Pawn* pawn);

    virtual void tick(){};
};
#endif //BUILDING_H