#include "FighterPawn.h"

#include "../../Core/Player.h"
#include "../Buildings/Building.h"
#include "../Entity.h"
#include "../ResourceEntity.h"
void DummyMonk::attack(Entity* attacked) {}

FighterPawnType DummyMonk::getType() { return FighterPawnType::DummyMonk; }

void DummySwordsman::attack(Entity* attacked) {}

FighterPawnType DummySwordsman::getType() { return FighterPawnType::DummySwordsman; }

FighterPawn* FighterPawn::createFighterPawn(FighterPawnType type, Building* placeOfCreation) {
    FighterPawn* newborn;
    switch (type) {
        case FighterPawnType::DummyMonk:
            newborn = new DummyMonk(Task(TaskID::Idle, placeOfCreation), false, Resource::DummyNothing, placeOfCreation->owner, placeOfCreation,
                                    placeOfCreation);
        case FighterPawnType::DummySwordsman:
            newborn = new DummySwordsman(Task(TaskID::Idle, placeOfCreation), false, Resource::DummyNothing, placeOfCreation->owner, placeOfCreation,
                                         placeOfCreation);
        default:
            throw("Type of FighterPawn not found");
    }
}

void FighterPawn::getResource(ResourceEntity* toGet) {
    if (positionBuilding != nullptr) GetOutOfBuilding();
    moveToResource(toGet);
    takePresentResource(toGet);
    moveToBuilding(owner->hub);
    positionBuilding = owner->hub;
    owner->hub->addPawn(this);
    drop(positionBuilding, position);
}

void FighterPawn::assignTask(const Task& task) { currentTask = task; }

DummyMonk::DummyMonk(Task task, bool _travelling, Resource resource, Player* Owner, Building* dest, Building* in) {
    currentTask = task;
    travelling = _travelling;
    holding = resource;
    owner = Owner;
    destination = dest;
    GetIntoBuilding(in);
}

DummySwordsman::DummySwordsman(Task task, bool _travelling, Resource resource, Player* Owner, Building* dest, Building* in) {
    currentTask = task;
    travelling = _travelling;
    holding = resource;
    owner = Owner;
    destination = dest;
    GetIntoBuilding(in);
}

void FighterPawn::moveToResource(ResourceEntity* toGet) { moveToPosition(toGet->position); }

void FighterPawn::takePresentResource(ResourceEntity* toTake) {
    holding = toTake->resource;
    toTake->destroy();
}

void FighterPawn::moveToPosition(Position pos) {
    GetOutOfBuilding();
    destinationPosition = pos;
}

void FighterPawn::moveToBuilding(Building* dest) {
    moveToPosition(dest->position);
    GetIntoBuilding(dest);
}

void FighterPawn::tick(double deltaTime) {
    if (fabs(position.x - destinationPosition.x) < 1e-6 && fabs(position.y - destinationPosition.y) > 1e-6) {
        position.x += (destination->position.x - destination->position.x) * speed * deltaTime;
        position.y += (destination->position.y - destination->position.y) * speed * deltaTime;

    } else
        travelling = false;
}

void FighterPawn::attack(Entity* attacked) {}

FighterPawnType FighterPawn::getType() { return FighterPawnType::DummNotFound; }
