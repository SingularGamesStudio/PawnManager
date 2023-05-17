#include "WorkerPawn.h"

#include <cmath>
#include <cstring>
#include <iostream>

#include "../../Core/Event.h"
#include "../../Core/Player.h"
#include "../../Core/godobject.h"
#include "../Buildings/Building.h"
#include "../ResourceEntity.h"


#ifdef SERVER_SIDE

void WorkerPawn::create(ptr<Building> placeOfCreation) {
    currentTask = Task(TaskID::Idle, placeOfCreation.dyn_cast<Entity>());
    travelling = false;
    holding = Resource::Nothing;
    needed = Resource::Nothing;
    owner = placeOfCreation->owner;
    IMHere(placeOfCreation);
    //godObject::global_server->sendPacketAll(Event(Event::Type::PAWN_APPEAR, id).getPacket());
}
void WorkerPawn::assignTask(const Task& toAssign) {
    currentTask = toAssign;
    toTake = false;
    toDrop = false;
    onTheWay.clear();
    currentInWay = 0;
    switch (toAssign.id) {
        case TaskID::Get:
            moveToBuilding(toAssign.destination.dyn_cast<Building>());
            needed = toAssign.object;
            toTake = true;
            break;
        case TaskID::Transport:
            moveToBuilding(toAssign.destination.dyn_cast<Building>());
            toTake = true;
            needed = toAssign.object;
            break;
        case TaskID::Move:
            moveToBuilding(toAssign.destination.dyn_cast<Building>());
            break;
        case TaskID::BeProcessed:
            moveToBuilding(toAssign.destination.dyn_cast<Building>());
            break;
        case TaskID::Idle:
            break;
        case TaskID::DropResource:
            toDrop = true;
            onTheWay.push_back(positionBuilding);
            break;
        default:
            throw("Unexpected WorkerPawn TaskID: ", toAssign.id);
    }
}
void WorkerPawn::tick(double deltaTime) {
    if (currentInWay < onTheWay.size()) {
        ptr<Building> dest = onTheWay[currentInWay];
        if (!dest) {
            assignTask(Task(TaskID::DropResource));
            return;
        }
        double signX = position.x - dest->position.x;
        double deltaX = fabs(position.x - dest->position.x);
        double signY = position.y - dest->position.y;
        double deltaY = fabs(position.y - dest->position.y);
        double wholeDelta = deltaX * deltaX + deltaY * deltaY;
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
        if (signX * (position.x - dest->position.x) <= 1 && signY * (position.y - dest->position.y) <= 1) {
            IMHere(dest);
            ++currentInWay;
            if (currentInWay < onTheWay.size()) {
                auto tmp = onTheWay[currentInWay]->position;
                double tim = std::hypot(tmp.x - position.x, tmp.y - position.y);
                tim /= speed;

                godObject::global_server->sendPacketAll(Event(Event::Type::PAWN_MOVE, id, onTheWay[currentInWay]->position, tim).getPacket());
            }
        }
    } else {
        travelling = false;
        onTheWay.clear();
        currentInWay = 0;
        bool temp = toTake;
        if (toDrop) {
            toDrop = false;
            drop(positionBuilding);
            return;
        }
        if (toTake) {
            if (positionBuilding->removeResource(needed)) {
                //godObject::global_server->sendPacketAll(Event(Event::Type::BUILDING_REMOVE_RES, positionBuilding->id, needed).getPacket());
                toTake = false;
                holding = needed;
                needed = Resource::Nothing;
                godObject::global_server->sendPacketAll(Event(Event::Type::PAWN_TAKE_RES, id, holding).getPacket());
            } else {
                owner->manager.cancelTask(currentTask, ptr<Pawn>(id));
                currentTask = TaskID::Idle;
            }
        }
        switch (currentTask.id) {
            case TaskID::Transport:
                if (temp) {
                    moveToBuilding(currentTask.destination2);
                    toDrop = true;
                } else {
                    owner->manager.finishTask(currentTask, ptr<Pawn>(id));
                    currentTask.id = TaskID::Idle;
                }
                break;
            case TaskID::BeProcessed:
                owner->manager.finishTask(currentTask, ptr<Pawn>(id));
                currentTask.id = TaskID::Craft;
                break;
            case TaskID::Craft:
                break;
            default:
                currentTask.id = TaskID::Idle;
                break;
        }
    }
}
void WorkerPawn::moveToBuilding(ptr<Building> dest) {
    travelling = true;
    std::unordered_map<ptr<Building>, std::vector<ptr<Building>>> visited;
    std::queue<ptr<Building>> q;
    visited[positionBuilding] = std::vector<ptr<Building>>();
    visited[positionBuilding].push_back(positionBuilding);
    q.push(positionBuilding);
    while (!q.empty()) {
        ptr<Building> currentB = q.front();
        q.pop();
        std::vector<ptr<Building>> way = visited[currentB];
        for (ptr<Building> toGo: currentB->children) {
            if (!visited[toGo].empty()) continue;
            visited[toGo] = way;
            visited[toGo].push_back(toGo);
            q.push(toGo);
        }
        if (!currentB->parent || !visited[currentB->parent].empty()) continue;
        visited[currentB->parent] = way;
        visited[currentB->parent].push_back(currentB->parent);
        q.push(currentB->parent);
    }
    for (ptr<Building> v: visited[dest]) { onTheWay.push_back(v); }
}
#endif
std::vector<uint8_t> WorkerPawn::serialize() const { return serializeSelf(); }

size_t WorkerPawn::deserialize(const uint8_t* data) { return deserializeSelf(data); }

std::vector<uint8_t> WorkerPawn::serializeSelf() const {
    std::vector<uint8_t> result = Pawn::serializeSelf();
    result.insert(result.begin(), DummyWorker);
    size_t size = sizeof(size_t) * 3 + sizeof(expertisesID) * expertises.size() + sizeof(ptr<Building>) * onTheWay.size();
    result.resize(result.size() + size);
    uint8_t* curr = result.data() + result.size() - size;
    curr += copyVariable(curr, currentInWay);
    curr += copySet(curr, expertises);
    curr += copyVector(curr, onTheWay);
    return result;
}

std::vector<uint8_t> WorkerPawn::serializeExpertises() const {
    size_t size = sizeof(int) + sizeof(size_t) + sizeof(expertisesID) * expertises.size();
    std::vector<uint8_t> result = std::vector<uint8_t>(size);
    uint8_t* curr = result.data();
    curr += copyVariable(curr, id);
    curr += copySet(curr, expertises);
    return result;
}

size_t WorkerPawn::updateExpertises(const uint8_t* data) {
    const uint8_t* curr = data;
    expertises.clear();
    curr += initializeSet(curr, expertises);
    return curr - data;
}

size_t WorkerPawn::deserializeSelf(const uint8_t* data) {
    size_t shift = Pawn::deserializeSelf(data);
    const uint8_t* curr = data + shift;
    curr += initializeVariable(curr, currentInWay);
    curr += initializeSet(curr, expertises);
    curr += initializeVector(curr, onTheWay);
    return curr - data;
}

WorkerPawn::~WorkerPawn() {
#ifdef SERVER_SIDE
    owner->manager.cancelTask(currentTask, ptr<Pawn>(id));
    drop(positionBuilding);
    IMNotHere();
#endif
}
