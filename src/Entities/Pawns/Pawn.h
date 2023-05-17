#ifndef PAWN_H
#define PAWN_H
///misha does not believe in me  :(
#include <utility>
#include <vector>

#include "../../Core/Resource.h"
#include "../../Core/Task.h"
#include "../Entity.h"
class Building;
class Pawn : public Entity {
public:
    Resource holding;
    Resource needed;
    ptr<Building> positionBuilding = ptr<Building>();
    Task currentTask;
    double speed = 111;
    bool travelling;
    bool toDrop;
    bool toTake;

#ifdef SERVER_SIDE
    void drop(ptr<Building> in, Position pos = {0, 0});
    void IMNotHere();
    void IMHere(ptr<Building> to);
    virtual void moveToPosition(Position toMove) = 0;
    virtual void assignTask(const Task& toAssign) = 0;
    void stopBeingIngridient();
    virtual void tick(double deltaTime);
#endif
    virtual ~Pawn();
    std::vector<uint8_t> serialize() const override;
    size_t deserialize(const uint8_t* data) override;

protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const uint8_t* data);
};
#endif//PAWN_H