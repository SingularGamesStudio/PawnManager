#include "FighterPawn.h"
#include "../Entity.h"
#include "../ResourceEntity.h"
#include "../Buildings/Building.h"
#include "../../Player.h"
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
        newborn = new DummyMonk(Task(TaskID::Idle, placeOfCreation), false, Resource::DummyNothing, placeOfCreation->owner, placeOfCreation, placeOfCreation);
    case FighterPawnType::DummySwordsman:
        newborn = new DummySwordsman(Task(TaskID::Idle, placeOfCreation), false, Resource::DummyNothing, placeOfCreation->owner, placeOfCreation, placeOfCreation);
    default:
        throw("Type of FighterPawn not found");
    }
}
void FighterPawn::getResource(ResourceEntity* toGet) {
    if (positionBuilding != nullptr)
        IMNotHere();
    moveToResource(toGet);
    takePresentResource(toGet);
    moveToBuilding(owner->hub);
    positionBuilding = owner->hub;
    owner->hub->addPawn(this);
    drop(positionBuilding, position);
}
void FighterPawn::assignTask(const Task& task) {
    currentTask = task;
}
DummyMonk::DummyMonk(Task task, bool BOOL, Resource resource, Player* Owner, Building* dest, Building* in) {
    currentTask = task;
    travelling = BOOL;
    holding = resource;
    owner = Owner;
    destination = dest;
    IMHere(in);
}
DummySwordsman::DummySwordsman(Task task, bool BOOL, Resource resource, Player* Owner, Building* dest, Building* in) {
    currentTask = task;
    travelling = BOOL;
    holding = resource;
    owner = Owner;
    destination = dest;
    IMHere(in);
}
void FighterPawn::moveToResource(ResourceEntity* toGet) {
    moveToPosition(toGet->position);
}
void FighterPawn::takePresentResource(ResourceEntity* toTake) {
    holding = toTake->resource;
    toTake->destroy();
}
void FighterPawn::moveToPosition(std::pair<double, double> pos) {
    IMNotHere();
    destinationPosition = pos;
}
void FighterPawn::moveToBuilding(Building* dest)  {
    moveToPosition(dest->position);
    IMHere(dest);
}
void FighterPawn::tick(double deltaTime) {
    if (fabs(position.first - destinationPosition.first) < 1e-6 && fabs(position.second - destinationPosition.second) > 1e-6){
    position.first += (destination->position.first - destination->position.first) *speed*deltaTime;
    position.second += (destination->position.second - destination->position.second) *speed*deltaTime;

    }
    else
    travelling = false;
}
void FighterPawn::attack(Entity* attacked) {};
FighterPawnType FighterPawn::getType() {
    return FighterPawnType::DummNotFound;
};
