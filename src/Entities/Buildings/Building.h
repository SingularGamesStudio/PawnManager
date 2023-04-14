#ifndef BUILDING_H
#define BUILDING_H
///misha does not believe in me  :(
#include <iostream>
#include <set>
#include <vector>

#include "../../Resource.h"
#include "../Entity.h"

class Pawn;

class Building : public virtual Entity {

public:
    constexpr static const double baseBuildingRadius = 30;
    Building(int id, std::pair<double, double> pos, ptr<Player> owner, double hp, double radius = baseBuildingRadius,
             ptr<Building> parent = ptr<Building>())
        : Entity(pos, owner, hp, radius) {
        this->parent = parent;
        this->id = id;
    }
#ifdef CLIENT_SIDE
    Building() {}
#endif
    ptr<Building> parent = ptr<Building>();
    std::set<ptr<Building>> children;
    std::multiset<Resource> resources;
    std::multiset<Resource> reservedResources;
    std::set<ptr<Pawn>> pawns;

    void addResource(Resource resource);

    bool removeResource(Resource resource);


    void addPawn(ptr<Pawn> pawn);

    void removePawn(ptr<Pawn> pawn);

#ifdef SERVER_SIDE
    virtual void tick(double deltaTime){};
#endif

    virtual ~Building();
};
#endif//BUILDING_H