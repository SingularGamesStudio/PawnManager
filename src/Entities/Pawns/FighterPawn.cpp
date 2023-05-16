#include "FighterPawn.h"

#include "../../Core/Event.h"
#include "../../Core/Player.h"
#include "../../Core/godobject.h"
#include "../Buildings/Building.h"
#include "../Entity.h"
#include "../ResourceEntity.h"


FighterPawnType Monk::getType() const { return FighterPawnType::Monk; }
FighterPawnType Swordsman::getType() const { return FighterPawnType::Swordsman; }
FighterPawnType FighterPawn::getType() const { return FighterPawnType::DummNotFound; };

Monk::Monk(int id, Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Building> dest, ptr<Building> in) {
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
Swordsman::Swordsman(int id, Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Building> dest, ptr<Building> in) {
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
        case FighterPawnType::Monk:
            newborn = (makeptr<Monk>(Task(TaskID::Idle, placeOfCreation), false, Resource::Nothing, placeOfCreation->owner, placeOfCreation,
                                     placeOfCreation))
                              .dyn_cast<FighterPawn>();
            break;
        case FighterPawnType::Swordsman:
            newborn = (makeptr<Swordsman>(Task(TaskID::Idle, placeOfCreation), false, Resource::Nothing, placeOfCreation->owner, placeOfCreation,
                                          placeOfCreation))
                              .dyn_cast<FighterPawn>();
            break;
        default:
            throw("Type of FighterPawn not found");
    }
    placeOfCreation->owner->pawns.insert(newborn->id);
    newborn->IMHere(placeOfCreation);
    //godObject::global_server->sendPacketAll(Event(Event::Type::PAWN_APPEAR, newborn->id).getPacket());
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
void FighterPawn::moveToPosition(Position pos) {
    IMNotHere();
    double tim = std::hypot(pos.x - position.x, pos.y - position.y);
    tim /= speed;
    godObject::global_server->sendPacketAll(Event(Event::Type::PAWN_MOVE, id, pos, tim).getPacket());
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
    Position dest = destinationPosition;
    double deltaX = fabs(position.x - dest.x);
    double deltaY = fabs(position.y - dest.y);
    double wholeDelta = deltaX * deltaX + deltaY * deltaY;
    if (toAttack && wholeDelta <= currentTask.destination->radius) {
        attack(currentTask.destination.dyn_cast<Entity>(), deltaTime);
        return;
    }
    if (travelling) {
        double signX = position.x - dest.x;
        double signY = position.y - dest.y;
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
            position.x += -signX * (deltaX / sqrt(wholeDelta)) * speed * deltaTime;
            position.y += -signY * (deltaY / sqrt(wholeDelta)) * speed * deltaTime;
        }
        //std::cerr<< position.x <<' '<< position.y <<'\n';
        //std::cerr<< dest->position.x <<' '<< dest->position.y <<'\n';
        if (signX * (position.x - dest.x) <= 1 && signY * (position.y - dest.y) <= 1) {
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
            needed = Resource::Nothing;
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

size_t FighterPawn::deserialize(const uint8_t* data) { return deserializeSelf(data); }

std::vector<uint8_t> FighterPawn::serializeSelf() const {
    std::vector<uint8_t> result = Pawn::serializeSelf();
    result.insert(result.begin(), static_cast<uint8_t>(getType()));
    size_t size = sizeof(double) * 2 + sizeof(bool) + sizeof(Position);
    result.resize(result.size() + size);
    uint8_t* curr = result.data() + result.size() - size;
    curr += copyVariable(curr, atk);
    curr += copyVariable(curr, speed);
    curr += copyVariable(curr, toAttack);
    curr += copyVariable(curr, destinationPosition);
    return result;
}

size_t FighterPawn::deserializeSelf(const uint8_t* data) {
    size_t shift = Pawn::deserializeSelf(data);
    const uint8_t* curr = data + shift;
    curr += initializeVariable(curr, atk);
    curr += initializeVariable(curr, speed);
    curr += initializeVariable(curr, toAttack);
    curr += initializeVariable(curr, destinationPosition);
    return curr - data;
}

FighterPawn::~FighterPawn() {
#ifdef SERVER_SIDE
    owner->manager.cancelTask(currentTask, ptr<Pawn>(id));
    drop(positionBuilding);
    godObject::global_server->sendPacketAll(Event(Event::Type::PAWN_DISAPPEAR, id).getPacket());
    IMNotHere();
#endif
}
