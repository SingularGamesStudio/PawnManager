#include "Building.h"
#include "Pawn.h"
#include <cmath>
enum class FighterPawnType{
    DummyMonk,
    DummySwordsman
};
class FighterPawn : public Pawn{
public:
    virtual FighterPawn* createFighterPawn(FighterPawnType type);
    double atk;
    double speed;
    void virtual attack(Entity* attacked);
    virtual FighterPawnType getType();
}
class DummyMonk : public FighterPawn{
public:
    void attack(Entity* attacked) override;
    FighterPawnType getType() override {
        return FighterPawnType::DummyMonk;
    }
};
class DummySwordsman : public FighterPawn{
public:
    void attack(Entity* attacked) override;
    FighterPawnType getType() override {
        return FighterPawnType::DummySwordsmans;
    }
};
static FighterPawn* FighterPawn::createFighterPawn(FighterPawnType type, Building* placeOfCreation){
    switch(id){
        case DummyMonk:
            this = new DummyMonk();
        case DummySwordsman:
            this = new DummySwordsman();
        default:
            throw("Type of FighterPawn not found");
    }
    currentTask = Task(TaskID::Idle, placeOfCreation);
    travelling = false;
    holding = nullptr;
    owner = placeOfCreation->owner;
    destination = placeOfCreation;
    inside = placeOfCreation;
    }
    void getResource(ResourceEntity* toGet) {
        if (inside != nullptr)
            ImNotHere(inside);
        moveToResource(toGet);
        takePresentResource(toGet);
        moveToBuilding(owner->hub);
        inside = hub;
        hub->addPawn(this);
        drop();
    }
    void moveToResource(ResourceEntity* toGet) {
        moveToPosition(toGet->position);
    }
    void takePresentResource(ResourceEntity* toTake) {
        holding = toTake->resource;
        toTake->destroy();
    }
    void moveToPosition(std::pair<double, double> pos) {
        IMNotHere(inside);
        double currentTime = 0;
        double timePerMove = sqrt(abs(position.first - pos.first) * abs(position.first - pos.first) +
            abs(position.second - pos.second) * abs(position.second - pos.second));
        while (currentTime < timePerMove) {
            currentTime += timePerMove / ticksPerSecond;
            position.first = currentTime / ticksPerSecond * (*positionBuilding).position.first
                + (1 - currentTime / ticksPerSecond) * (*positionBuilding).position.first;
            position.second = currentTime / ticksPerSecond * (*dest).position.second
                + (1 - currentTime / ticksPerSecond) * (*dest).position.second;
        }
        position = pos;

    }
    void moveToBuilding(Building* dest) override {
        moveToPosition(dest->position);
        IMHere(dest);
    }
}
