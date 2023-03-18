#ifndef FIGHTERPAWN_H
#define FIGHTERPAWN_H
///misha does not believe in me  :(
#include "Pawn.h"
#include <cmath>
#include <vector>
enum class FighterPawnType{
    DummyMonk,
    DummySwordsman
};
class Entity;
class ResourceEntity;
class FighterPawn : public Pawn{
public:
    double atk;
    double speed;
    std::pair<double, double> destinationPosition;
    virtual void attack(Entity* attacked);
    virtual FighterPawnType getType();
    static FighterPawn* createFighterPawn(FighterPawnType type, Building* placeOfCreation);
    void moveToResource(ResourceEntity* toGet);
    void takePresentResource(ResourceEntity* toTake);
    void moveToPosition(std::pair<double, double> pos);
    void moveToBuilding(Building* dest) override;
    void getResource(ResourceEntity* toGet);
    void assignTask(const Task& task);
    void tick();
};
class DummyMonk : public FighterPawn {
public:
    void attack(Entity* attacked) override;
    FighterPawnType getType() override;
    DummyMonk(Task task, bool BOOL, Resource resource, Player* Owner, Building* dest, Building* in);
};
class DummySwordsman : public FighterPawn {
public:
    void attack(Entity* attacked) override;
    FighterPawnType getType() override;
    DummySwordsman(Task task, bool BOOL, Resource resource, Player* Owner, Building* dest, Building* in);
};
#endif //FIGHTERPAWN_H