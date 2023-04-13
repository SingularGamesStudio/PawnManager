#include "FighterPawn.h"

#include "../../Player.h"
#include "../Buildings/Building.h"
#include "../Entity.h"
#include "../ResourceEntity.h"
FighterPawnType DummyMonk::getType() { return FighterPawnType::DummyMonk; }
FighterPawnType DummySwordsman::getType() { return FighterPawnType::DummySwordsman; }
ptr<FighterPawn> FighterPawn::createFighterPawn(FighterPawnType type, ptr<Building> placeOfCreation) {
    ptr<FighterPawn> newborn;
    switch (type) {
        case FighterPawnType::DummyMonk:
            newborn = static_cast<ptr<FighterPawn>>(makeptr<DummyMonk>(Task(TaskID::Idle, placeOfCreation), false, Resource::DummyNothing,
                                                                       placeOfCreation->owner, placeOfCreation, placeOfCreation));
        case FighterPawnType::DummySwordsman:
            newborn = static_cast<ptr<FighterPawn>>(makeptr<DummySwordsman>(Task(TaskID::Idle, placeOfCreation), false, Resource::DummyNothing,
                                                                            placeOfCreation->owner, placeOfCreation, placeOfCreation));
        default:
            throw("Type of FighterPawn not found");
    }
}
void FighterPawn::getResource(ResourceEntity* toGet) {
    if (positionBuilding) IMNotHere();
    moveToResource(toGet);
    takePresentResource(toGet);
    moveToBuilding(owner->hub);
    positionBuilding = owner->hub;
    owner->hub->addPawn(ptr<Pawn>(id));
    drop(positionBuilding, position);
}
void FighterPawn::assignTask(const Task& toAssign) {
    currentTask = toAssign;
    switch (toAssign.id) {
        case TaskID::Get:
            moveToBuilding(toAssign.destination);
            needed = toAssign.object;
            toTake = true;
            break;
        case TaskID::Transport:
            moveToBuilding(toAssign.destination);
            toTake = true;
            needed = toAssign.object;
            break;
        case TaskID::Move:
            moveToBuilding(toAssign.destination);
            break;
        case TaskID::BeProcessed:
            moveToBuilding(toAssign.destination);
            break;
        case TaskID::Idle:
            break;
        case TaskID::Attack:
            toAttack = true;
            moveToBuilding(toAssign.destination);
        default:
            throw("Unexpected FighterPawn TaskID: ", toAssign.id);
    }
}
DummyMonk::DummyMonk(int id, Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Building> dest, ptr<Building> in) {
    this->id = id;
    currentTask = task;
    travelling = BOOL;
    holding = resource;
    owner = Owner;
    destination = dest;
    IMHere(in);
}
DummySwordsman::DummySwordsman(int id, Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Building> dest, ptr<Building> in) {
    this->id = id;
    currentTask = task;
    travelling = BOOL;
    holding = resource;
    owner = Owner;
    destination = dest;
    IMHere(in);
}
void FighterPawn::moveToResource(ResourceEntity* toGet) { moveToPosition(toGet->position); }
void FighterPawn::takePresentResource(ResourceEntity* toTake) {
    holding = toTake->resource;
    //toTake->destroy();
}
void FighterPawn::moveToPosition(std::pair<double, double> pos) {
    IMNotHere();
    destinationPosition = pos;
}
void FighterPawn::moveToBuilding(ptr<Building> dest) {
    moveToPosition(dest->position);
    IMHere(dest);
}
void FighterPawn::tick(double deltaTime) {
    std::pair<double, double> dest = destinationPosition;
    double deltaX = fabs(position.first - dest.first);
    double deltaY = fabs(position.second - dest.second);
    double wholeDelta = deltaX * deltaX + deltaY * deltaY;
    if (toAttack && wholeDelta <= currentTask.destination->radius) {
        attack(static_cast<ptr<Entity>>(currentTask.destination));
        if (currentTask.destination->hp <= 0) {
            toAttack = false;
            currentTask = Task(TaskID::Move, owner->hub);
        }
    }
    if (travelling) {
        double signX = position.first - dest.first;
        double signY = position.second - dest.second;
        if (signX < -1e-2) {
            signX = -1;
        } else if (signX > 1e-2) {
            signX = 1;
        } else
            signX = 0;
        if (signY < -1e-2) {
            signY = -1;
        } else if (signY > 1e-2) {
            signY = 1;
        } else
            signY = 0;
        if (wholeDelta > 1e-7) {
            position.first += -signX * (deltaX / sqrt(wholeDelta)) * speed * deltaTime;
            position.second += -signY * (deltaY / sqrt(wholeDelta)) * speed * deltaTime;
        }
        //std::cerr<< position.first <<' '<< position.second <<'\n';
        //std::cerr<< dest->position.first <<' '<< dest->position.second <<'\n';
        if (signX * (position.first - dest.first) <= 1 && signY * (position.second - dest.second) <= 1) {
            if (!destination) IMHere(destination);
        }
    } else {
        travelling = false;
        if (toDrop) {
            toDrop = false;
            drop(positionBuilding);
        }
        if (toTake) {
            toTake = false;
            holding = needed;
            needed = Resource::DummyNothing;
            ///TODO removeFromExistence needed
            if (!toDrop) currentTask = TaskID::Idle;
        }
        switch (currentTask.id) {
            default:
                owner->manager.finishTask(currentTask, ptr<Pawn>(id));
                currentTask.id = TaskID::Idle;
        }
    }
}
void FighterPawn::attack(ptr<Entity> attacked) { attacked->hp -= atk; };
FighterPawnType FighterPawn::getType() { return FighterPawnType::DummNotFound; };

FighterPawn::~FighterPawn() {
    owner->manager.cancelTask(currentTask, ptr<Pawn>(id));
    if (holding != Resource::DummyNothing)
        if (positionBuilding) positionBuilding->addResource(holding);
        else
            makeptr<ResourceEntity>(holding, position);
    IMNotHere();
}