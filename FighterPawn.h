#ifndef FIGHTERPAWN_H
#define FIGHTERPAWN_H
///misha does not believe in me  :(
#include "Pawn.h"
#include <cmath>
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
    void virtual attack(Entity* attacked);
    virtual FighterPawnType getType();
    static FighterPawn* createFighterPawn(FighterPawnType type, Building* placeOfCreation);
    void moveToResource(ResourceEntity* toGet);
    void takePresentResource(ResourceEntity* toTake);
    void moveToPosition(std::pair<double, double> pos);
    void moveToBuilding(Building* dest) override;
    void getResource(ResourceEntity* toGet);
};
class DummyMonk : public FighterPawn {
public:
    void attack(Entity* attacked) override;
    DummyMonk();
    FighterPawnType getType() override;
};
class DummySwordsman : public FighterPawn {
public:
    void attack(Entity* attacked) override;
    DummySwordsman();
    FighterPawnType getType() override;
};
#endif //FIGHTERPAWN_H