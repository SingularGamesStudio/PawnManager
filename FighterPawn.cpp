#include "FighterPawn.h"
#include "Entity.h"
#include "ResourceEntity.h"
#include "Building.h"
#include "Player.h"
void DummyMonk::attack(Entity* attacked) {
    
}
FighterPawnType DummyMonk::getType()  {
    return FighterPawnType::DummyMonk;
}
void DummySwordsman::attack(Entity* attacked)  {
    
}
FighterPawnType DummySwordsman::getType()  {
    return FighterPawnType::DummySwordsman;
}
FighterPawn* FighterPawn::createFighterPawn(FighterPawnType type, Building* placeOfCreation) {
    FighterPawn* newborn;
    switch (type) {
        case FighterPawnType::DummyMonk:
        newborn = new DummyMonk();
        case FighterPawnType::DummySwordsman:
        newborn = new DummySwordsman();
    default:
        throw("Type of FighterPawn not found");
    }
    newborn->currentTask = Task(TaskID::Idle, placeOfCreation);
    newborn->travelling = false;
    newborn->holding = Resource::DummyNothing;
    newborn->owner = placeOfCreation->owner;
    newborn->destination = placeOfCreation;
    newborn->inside = placeOfCreation;
}
void FighterPawn::getResource(ResourceEntity* toGet) {
    if (inside != nullptr)
        IMNotHere(inside);
    moveToResource(toGet);
    takePresentResource(toGet);
    moveToBuilding(owner->hub);
    inside = owner->hub;
    owner->hub->addPawn(this);
    drop(inside, position);
}
void FighterPawn::moveToResource(ResourceEntity* toGet) {
    moveToPosition(toGet->position);
}
void FighterPawn::takePresentResource(ResourceEntity* toTake) {
    holding = toTake->resource;
    toTake->destroy();
}
void FighterPawn::moveToPosition(std::pair<double, double> pos) {
    IMNotHere(inside);
    double currentTime = 0;
    double timePerMove = sqrt(abs(position.first - pos.first) * abs(position.first - pos.first) +
        abs(position.second - pos.second) * abs(position.second - pos.second));
    while (currentTime < timePerMove) {
        currentTime += timePerMove / ticksPerSecond;
        position.first = currentTime / ticksPerSecond * pos.first
            + (1 - currentTime / ticksPerSecond) * pos.first;
        position.second = currentTime / ticksPerSecond * pos.second
            + (1 - currentTime / ticksPerSecond) * pos.second;
    }
    position = pos;

}
void FighterPawn::moveToBuilding(Building* dest)  {
    moveToPosition(dest->position);
    IMHere(dest);
}
