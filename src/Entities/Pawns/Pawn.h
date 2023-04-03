#ifndef PAWN_H
#define PAWN_H
///misha does not believe in me  :(
#include <utility>
#include "../../Resource.h"
#include "../../Task.h"
#include "../Entity.h"
#include <vector>
class Building;
class Pawn : public Entity {
public:
    Resource holding;
    Resource needed;
    ptr<Building> destination;//removed it from use in workerPawn, maybe define it in Fighterpawn only?
    ptr<Building> positionBuilding = ptr<Building>();
    Task currentTask;
    bool travelling;
    double speed = 111;
    bool toDrop;
    bool toTake;
    void drop(ptr<Building> in, std::pair<double, double> pos = { 0,0 });
    void destroy();
    void IMNotHere();
    void IMHere(ptr<Building> to);
    virtual void moveToBuilding(ptr<Building> toMove) = 0;
    virtual void moveToPosition(std::pair<double, double> toMove) = 0;
    virtual void assignTask(const Task& toAssign) = 0;
    void stopBeingIngridient();
    virtual void tick(double deltaTime);
    virtual std::vector<uint8_t> serialize() const;
    virtual void deserialize(const std::vector<uint8_t>& data) const;
};
#endif //PAWN_H