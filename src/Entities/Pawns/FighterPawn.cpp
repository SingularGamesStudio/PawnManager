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
    if (travelling){
        std::pair<double, double>  dest = destinationPosition;
        double signX = position.first - dest.first;
        double signY = position.second - dest.second;
        double signX = position.first - dest.first;
        double deltaX = fabs(position.first - dest.first);
        double signY = position.second - dest.second;
        double deltaY = fabs(position.second - dest.second);
        double wholeDelta = deltaX * deltaX + deltaY * deltaY;
        if (signX < -1e-2){
            signX = -1;
        } else if (signX > 1e-2){
            signX = 1;
        } else
            signX = 0;
        if (signY < -1e-2){
            signY = -1;
        } else if (signY > 1e-2){
            signY = 1;
        } else
            signY = 0;
        if (wholeDelta > 1e-7) {
            position.first += -signX * (deltaX / sqrt(wholeDelta)) * speed * deltaTime;
            position.second += -signY * (deltaY / sqrt(wholeDelta)) * speed * deltaTime;
        }
        //std::cerr<< position.first <<' '<< position.second <<'\n';
        //std::cerr<< dest->position.first <<' '<< dest->position.second <<'\n';
        if (signX * (position.first - dest.first) <= 1 && signY * (position.second - dest.second) <= 1){
            if(destination != nullptr)
                IMHere(destination);
        }
    }
    else {
        travelling = false;
        if (toDrop) {
            toDrop = false;
            drop(positionBuilding);
        }
        if (toTake) {
            toTake = false;
            holding = needed;
            needed = Resource::DummyNothing;
            ///TO DO removeFromExistence needed
            if(!toDrop)
                currentTask = TaskID::Idle;
        }
        switch (currentTask.id) {
            default:
                currentTask.id = TaskID::Idle;
        }
    }
}
void FighterPawn::attack(Entity* attacked) {};
FighterPawnType FighterPawn::getType() {
    return FighterPawnType::DummNotFound;
};
