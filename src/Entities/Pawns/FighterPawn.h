#ifndef FIGHTERPAWN_H
#define FIGHTERPAWN_H
///misha does not believe in me  :(
#include <cmath>
#include <vector>

#include "Pawn.h"
enum class FighterPawnType : uint8_t { Monk, Swordsman, DummNotFound };
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

    virtual FighterPawnType getType() const;

    #ifdef SERVER_SIDE
    virtual void attack(ptr<Entity> attacked, double deltaTime);
    static ptr<FighterPawn> createFighterPawn(FighterPawnType type, ptr<Building> placeOfCreation);
    void moveToResource(ResourceEntity* toGet);
    void takePresentResource(ResourceEntity* toTake);
    void moveToPosition(std::pair<double, double> pos);
    void moveToBuilding(ptr<Building> dest) override;
    void getResource(ResourceEntity* toGet);
    void assignTask(const Task& task) override;
    void tick(double deltaTime) override;
#endif
    virtual ~FighterPawn();
    std::vector<uint8_t> serialize() const override;

    size_t deserialize(const std::vector<uint8_t>& data) override;
protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const std::vector<uint8_t> &data);
};

class Monk : public FighterPawn {
public:
    //void attack(ptr<Entity> attacked) override;

    FighterPawnType getType() const override;
    Monk(int id, Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Building> dest, ptr<Building> in);
};
class Swordsman : public FighterPawn {
public:
    //void attack(ptr<Entity> attacked) override;

    FighterPawnType getType() const override;
    Swordsman(int id, Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Building> dest, ptr<Building> in);
};
#endif//FIGHTERPAWN_H