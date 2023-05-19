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

Monk::Monk(int id, Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Entity> dest, ptr<Building> in) {
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
Swordsman::Swordsman(int id, Task task, bool BOOL, Resource resource, ptr<Player> Owner, ptr<Entity> dest, ptr<Building> in) {
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
    attacked->changeHealth(-atk * deltaTime);
    if (attacked.dyn_cast<FighterPawn>()) {
        ptr<FighterPawn> fighterPawnAttacked = attacked.dyn_cast<FighterPawn>();
        changeHealth(-(fighterPawnAttacked->atk) * deltaTime * (1 - defendModifier));
        attacked->changeHealth(-atk * deltaTime * defendModifier);
    }
};

ptr<FighterPawn> FighterPawn::createFighterPawn(FighterPawnType type, ptr<Building> placeOfCreation) {
    ptr<FighterPawn> newborn;
    switch (type) {
        case FighterPawnType::Monk:
            newborn = (makeptr<Monk>(Task(TaskID::Idle, placeOfCreation.dyn_cast<Entity>()), false, Resource::Nothing, placeOfCreation->owner,
                                     placeOfCreation.dyn_cast<Entity>(), placeOfCreation))
                              .dyn_cast<FighterPawn>();
            break;
        case FighterPawnType::Swordsman:
            newborn = (makeptr<Swordsman>(Task(TaskID::Idle, placeOfCreation.dyn_cast<Entity>()), false, Resource::Nothing, placeOfCreation->owner,
                                          placeOfCreation.dyn_cast<Entity>(), placeOfCreation))
                              .dyn_cast<FighterPawn>();
            newborn->atk = 30;
            break;
        default:
            throw std::logic_error("Type of FighterPawn not found");
    }
    placeOfCreation->owner->pawns.insert(newborn->id);
    newborn->IMHere(placeOfCreation);
    return ptr<FighterPawn>(newborn->id);
}
void FighterPawn::getResource(ResourceEntity* toGet) {
    if (positionBuilding) IMNotHere();
    moveToResource(toGet);
    takePresentResource(toGet);
    moveToEntity(owner->hub.dyn_cast<Entity>());
    positionBuilding = owner->hub;
    owner->hub->addPawn(ptr<Pawn>(id));
    drop(positionBuilding, position);
}
void FighterPawn::assignTask(const Task& toAssign) {
    currentTask = toAssign;
    switch (toAssign.id) {
        case TaskID::Get:
            moveToEntity(toAssign.destination.dyn_cast<Entity>());
            needed = toAssign.object;
            toTake = true;
            break;
        case TaskID::Transport:
            moveToEntity(toAssign.destination.dyn_cast<Entity>());
            toTake = true;
            needed = toAssign.object;
            break;
        case TaskID::Move:
            moveToEntity(toAssign.destination.dyn_cast<Entity>());
            break;
        case TaskID::BeProcessed:
            moveToEntity(toAssign.destination.dyn_cast<Entity>());
            break;
        case TaskID::Idle:
            break;
        case TaskID::Attack:
            toAttack = true;
            moveToEntity(toAssign.destination.dyn_cast<Entity>());
            break;
        case TaskID::Protect:
            travelling = true;
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
    travelling = true;
    destinationPosition = pos;
    destination = ptr<Entity>();
}
void FighterPawn::moveToEntity(ptr<Entity> dest) {
    //moveToPosition(dest->position);
    destination = dest.dyn_cast<Entity>();
    travelling = true;
    destinationPosition = Position(-1, -1);
    godObject::global_server->sendPacketAll(Event(Event::Type::PAWN_MOVE, id, destination.id, speed).getPacket());
}
void FighterPawn::tick(double deltaTime) {
    if (!currentTask.destination) toAttack = false;
    if ((!currentTask.destination) && (!currentTask.destination2) && (currentTask.id == TaskID::Protect || currentTask.id == TaskID::Attack)) {
        toAttack = false;
        assignTask(Task(TaskID::Move, owner->hub.dyn_cast<Entity>()));
        return;
    }
    if (currentTask.id == TaskID::Protect) {
        ptr<Entity> enemy;
        for (auto theOpponent: godObject::global_server->players) {
            if (theOpponent.second == owner) continue;
            for (auto thePawnOfOpponent: theOpponent.second->pawns) {
                if (thePawnOfOpponent.dyn_cast<FighterPawn>()) {
                    if (dist(currentTask.destination2->position, thePawnOfOpponent->position) <= awarenessRadius) {
                        enemy = thePawnOfOpponent.dyn_cast<Entity>();
                        break;
                    }
                }
            }
            if (enemy) break;
        }
        if (enemy) {
            currentTask.destination = enemy;
            destination = enemy;
            toAttack = true;
        } else {
            if (dist(position, currentTask.destination2->position) > 1) { moveToEntity(currentTask.destination2.dyn_cast<Entity>()); }
        }
    }
    if (currentTask.id == TaskID::Craft) return;
    Position dest;
    if (destination) dest = destination->position;
    else
        dest = destinationPosition;
    double deltaX = fabs(position.x - dest.x);
    double deltaY = fabs(position.y - dest.y);
    double wholeDelta = std::sqrt(deltaX * deltaX + deltaY * deltaY);
    if (toAttack && wholeDelta <= currentTask.destination->radius) {
        attack(currentTask.destination.dyn_cast<Entity>(), deltaTime);
        return;
    } else if (toAttack) {
        moveToPosition(currentTask.destination->position);
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
        if (signX * (position.x - dest.x) <= 1 && signY * (position.y - dest.y) <= 1) {
            travelling = false;
            position = dest;
        }
    } else {
        if (toDrop) {
            toDrop = false;
            drop(positionBuilding);
        }
        if (toTake) {
            toTake = false;
            holding = needed;
            needed = Resource::Nothing;
        }
        if (currentTask.id == TaskID::BeProcessed) {
            owner->manager.finishTask(currentTask, ptr<Pawn>(id));
            currentTask.id = TaskID::Craft;
        }
        if (currentTask.id != TaskID::Protect && currentTask.id != TaskID::Craft) {
            currentTask = TaskID::Idle;
            owner->manager.finishTask(currentTask, ptr<Pawn>(id));
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
