#ifndef PAWN_H
#define PAWN_H
///misha does not believe in me  :(
#include <utility>
#include "../../Resource.h"
#include "../../Task.h"
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
    double speed = 111;
    bool toDrop;
    bool toTake;
    void drop(Building* in, std::pair<double, double> pos = { 0,0 });
    void destroy();
    void IMNotHere();
    void IMHere(Building* to);
    virtual void moveToBuilding(Building* toMove) = 0;
    virtual void assignTask(const Task& toAssign) = 0;
    void beIngridient();
    void stopBeingIngridient();
    virtual void tick(double deltaTime);
};
#endif //PAWN_H