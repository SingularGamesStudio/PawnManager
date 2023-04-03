#ifndef FIGHTERPAWN_H
#define FIGHTERPAWN_H
///misha does not believe in me  :(
#include "Pawn.h"
#include <cmath>
#include <vector>
enum class FighterPawnType{
    DummyMonk,
    DummySwordsman,
    DummNotFound
};
class Entity;
class ResourceEntity;
class FighterPawn : public Pawn{
public:
    double atk;
    double speed;
    bool toAttack = false;
    std::pair<double, double> destinationPosition;
    virtual void attack(ptr<Entity> attacked);
    virtual FighterPawnType getType();
    static ptr<FighterPawn> createFighterPawn(FighterPawnType type, ptr<Building> placeOfCreation);
    void moveToResource(ResourceEntity* toGet);
    void takePresentResource(ResourceEntity* toTake);
    void moveToPosition(std::pair<double, double> pos);
    void moveToBuilding(ptr<Building> dest) override;
    void getResource(ResourceEntity* toGet);
    void assignTask(const Task& task) override;
    void tick(double deltaTime) override;
};
class DummyMonk : public FighterPawn {
public:
    //void attack(ptr<Entity> attacked) override;
    FighterPawnType getType() override;
    DummyMonk(Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Building> dest, ptr<Building> in);
};
class DummySwordsman : public FighterPawn {
public:
    //void attack(ptr<Entity> attacked) override;
    FighterPawnType getType() override;
    DummySwordsman(Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Building> dest, ptr<Building> in);
};
#endif //FIGHTERPAWN_H