#ifndef FIGHTERPAWN_H
#define FIGHTERPAWN_H
///misha does not believe in me  :(
#include <cmath>
#include <vector>

#include "Pawn.h"
enum class FighterPawnType : uint8_t { DummyMonk, DummySwordsman, DummNotFound };
class Entity;
class ResourceEntity;
class FighterPawn : public Pawn {
public:
    double atk = 10;
    double speed = 100;
    double hp = 30;
    bool toAttack = false;
    std::pair<double, double> destinationPosition;
    ptr<Building> destination;
    #ifdef SERVER_SIDE
    virtual void attack(ptr<Entity> attacked, double deltaTime);
    virtual FighterPawnType getType() const;
    static ptr<FighterPawn> createFighterPawn(FighterPawnType type, ptr<Building> placeOfCreation);
    void moveToResource(ResourceEntity* toGet);
    void takePresentResource(ResourceEntity* toTake);
    void moveToPosition(std::pair<double, double> pos);
    void moveToBuilding(ptr<Building> dest) override;
    void getResource(ResourceEntity* toGet);
    void assignTask(const Task& task) override;
    void tick(double deltaTime) override;
    virtual ~FighterPawn();
    std::vector<uint8_t> serialize() const override;
    #endif
    size_t deserialize(const std::vector<uint8_t>& data) override;
protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const std::vector<uint8_t> &data);
};

class DummyMonk : public FighterPawn {
public:
    //void attack(ptr<Entity> attacked) override;
    #ifdef SERVER_SIDE
    FighterPawnType getType() const override;
    DummyMonk(int id, Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Building> dest, ptr<Building> in);
    #endif
};
class DummySwordsman : public FighterPawn {
public:
    //void attack(ptr<Entity> attacked) override;
    #ifdef SERVER_SIDE
    FighterPawnType getType() const override;
    DummySwordsman(int id, Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Building> dest, ptr<Building> in);
    #endif
};
#endif//FIGHTERPAWN_H