#include "FighterPawn.h"

#include "../../Player.h"
#include "../Buildings/Building.h"
#include "../Entity.h"
#include "../ResourceEntity.h"


FighterPawnType DummyMonk::getType() const { return FighterPawnType::DummyMonk; }
FighterPawnType DummySwordsman::getType() const { return FighterPawnType::DummySwordsman; }
FighterPawnType FighterPawn::getType() const { return FighterPawnType::DummNotFound; };

DummyMonk::DummyMonk(int id, Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Building> dest, ptr<Building> in) {
    this->id = id;
    currentTask = task;
    travelling = BOOL;
    holding = resource;
    owner = Owner;
    destination = dest;
#ifdef SERVER_SIDE
    IMHere(in);
#endif
}
DummySwordsman::DummySwordsman(int id, Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Building> dest, ptr<Building> in) {
    this->id = id;
    currentTask = task;
    travelling = BOOL;
    holding = resource;
    owner = Owner;
    destination = dest;
#ifdef SERVER_SIDE
    IMHere(in);
#endif
}
#ifdef SERVER_SIDE
void FighterPawn::attack(ptr<Entity> attacked, double deltaTime) {
    std::cout << "Genuinly attacking " << -atk << " " << attacked->hp << "\n";
    attacked->changeHealth(-atk * deltaTime);
    if (attacked.dyn_cast<FighterPawn>()) {
        ptr<FighterPawn> fighterPawnAttacked = attacked.dyn_cast<FighterPawn>();
        changeHealth(-(fighterPawnAttacked->atk) * deltaTime);
    }
};

ptr<FighterPawn> FighterPawn::createFighterPawn(FighterPawnType type, ptr<Building> placeOfCreation) {
    ptr<FighterPawn> newborn;
    switch (type) {
        case FighterPawnType::DummyMonk:
            newborn = (makeptr<DummyMonk>(Task(TaskID::Idle, placeOfCreation), false, Resource::DummyNothing, placeOfCreation->owner, placeOfCreation,
                                          placeOfCreation))
                              .dyn_cast<FighterPawn>();
            break;
        case FighterPawnType::DummySwordsman:
            newborn = (makeptr<DummySwordsman>(Task(TaskID::Idle, placeOfCreation), false, Resource::DummyNothing, placeOfCreation->owner,
                                               placeOfCreation, placeOfCreation))
                              .dyn_cast<FighterPawn>();
            break;
        default:
            throw("Type of FighterPawn not found");
    }
    placeOfCreation->owner->pawns.insert(newborn->id);
    newborn->IMHere(placeOfCreation);
    global_server->sendPacketAll(Event(Event::Type::PAWN_APPEAR, id).getPacket());
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
            break;
        case TaskID::Protect:
            break;
        default:
            throw("Unexpected FighterPawn TaskID: ", toAssign.id);
    }
}

void FighterPawn::moveToResource(ResourceEntity* toGet) { moveToPosition(toGet->position); }
void FighterPawn::takePresentResource(ResourceEntity* toTake) {
    holding = toTake->resource;
    //toTake->destroy();
}
void FighterPawn::moveToPosition(std::pair<double, double> pos) {
    IMNotHere();
    global_server->sendPacketAll(Event(Event::Type::PAWN_MOVE, id, pos).getPacket());
    travelling = true;
    destinationPosition = pos;
}
void FighterPawn::moveToBuilding(ptr<Building> dest) { moveToPosition(dest->position); }
void FighterPawn::tick(double deltaTime) {
    if (!currentTask.destination) {
        toAttack = false;
        currentTask = Task(TaskID::Move, owner->hub);
        return;
    }
    if (currentTask.id == TaskID::Protect) {
        ptr<Entity> enemy;
        ///TODO enemy spotted
        if (enemy) {
            attack(enemy, deltaTime);
        } else
            moveToBuilding(currentTask.destination);
    }
    std::pair<double, double> dest = destinationPosition;
    double deltaX = fabs(position.first - dest.first);
    double deltaY = fabs(position.second - dest.second);
    double wholeDelta = deltaX * deltaX + deltaY * deltaY;
    if (toAttack && wholeDelta <= currentTask.destination->radius) {
        attack(currentTask.destination.dyn_cast<Entity>(), deltaTime);
        return;
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
            travelling = false;
            position = dest;
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
#endif
std::vector<uint8_t> FighterPawn::serialize() const { return serializeSelf(); }

size_t FighterPawn::deserialize(const std::vector<uint8_t>& data) { return deserializeSelf(data); }

std::vector<uint8_t> FighterPawn::serializeSelf() const {
    std::vector<uint8_t> result = Pawn::serializeSelf();
    result.insert(result.begin(), static_cast<uint8_t>(getType()));
    size_t size = sizeof(double) * 2 + sizeof(bool) + sizeof(std::pair<double, double>);
    result.resize(result.size() + size);
    uint8_t* curr = result.data() + result.size();
    curr += copyVariable(curr, atk);
    curr += copyVariable(curr, speed);
    curr += copyVariable(curr, toAttack);
    curr += copyVariable(curr, destinationPosition);
    return result;
}

size_t FighterPawn::deserializeSelf(const std::vector<uint8_t>& data) {
    size_t shift = Pawn::deserializeSelf(data);
    const uint8_t* curr = data.data() + shift;
    curr += initializeVariable(curr, atk);
    curr += initializeVariable(curr, speed);
    curr += initializeVariable(curr, toAttack);
    curr += initializeVariable(curr, destinationPosition);
    return curr - data.data();
}

FighterPawn::~FighterPawn() {
#ifdef SERVER_SIDE
    owner->manager.cancelTask(currentTask, ptr<Pawn>(id));
    drop(buildingPosition);
    global_server->sendPacketAll(Event(Event::Type::PAWN_DISAPPEAR, id);.getPacket);
    IMNotHere();
#endif
}
