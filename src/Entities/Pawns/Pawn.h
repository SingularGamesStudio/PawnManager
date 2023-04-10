#ifndef PAWN_H
#define PAWN_H

#include <utility>

#include "../../Core/Resource.h"
#include "../../Core/Task.h"
#include "../Entity.h"

class Building;

class Pawn : public Entity {
public:
    Resource holding;
    Resource needed;
    Building* destination;//removed it from use in workerPawn, maybe define it in Fighterpawn only?
    Building* positionBuilding = nullptr;
    Task currentTask;
    bool travelling;
    double speed = 0.3;

    void drop(Building* in, Position pos = {0, 0});

    void destroy();

    void GetOutOfBuilding();
    void GetIntoBuilding(Building* to);

    virtual void moveToBuilding(Building* toMove) = 0;

    virtual void assignTask(const Task& toAssign) = 0;

    void beIngridient();
    void stopBeingIngridient();

    virtual void tick(double deltaTime);
};
#endif//PAWN_H