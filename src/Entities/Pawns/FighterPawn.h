#ifndef FIGHTERPAWN_H
#define FIGHTERPAWN_H

#include <cmath>
#include <vector>

#include "Pawn.h"

enum class FighterPawnType { DummyMonk, DummySwordsman, DummNotFound };

class Entity;

class ResourceEntity;

class FighterPawn : public Pawn {
public:
    double atk;
    double speed;
    Position destinationPosition;

    virtual void attack(Entity* attacked);

    virtual FighterPawnType getType();

    static FighterPawn* createFighterPawn(FighterPawnType type, Building* placeOfCreation);

    void moveToResource(ResourceEntity* toGet);
    void takePresentResource(ResourceEntity* toTake);

    void moveToPosition(Position pos);
    void moveToBuilding(Building* dest) override;

    void getResource(ResourceEntity* toGet);

    void assignTask(const Task& task) override;

    void tick(double deltaTime) override;
};
class DummyMonk : public FighterPawn {
public:
    void attack(Entity* attacked) override;
    FighterPawnType getType() override;
    DummyMonk(Task task, bool _travelling, Resource resource, Player* Owner, Building* dest, Building* in);
};
class DummySwordsman : public FighterPawn {
public:
    void attack(Entity* attacked) override;
    FighterPawnType getType() override;
    DummySwordsman(Task task, bool _travelling, Resource resource, Player* Owner, Building* dest, Building* in);
};
#endif//FIGHTERPAWN_H