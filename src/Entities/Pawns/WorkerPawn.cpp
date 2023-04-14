#include "WorkerPawn.h"

#include <cmath>
#include <cstring>
#include <iostream>

#include "../../Event.h"
#include "../../Player.h"
#include "../../godobject.h"
#include "../Buildings/Building.h"
#include "../ResourceEntity.h"


#ifdef SERVER_SIDE

void WorkerPawn::create(ptr<Building> placeOfCreation) {
    currentTask = Task(TaskID::Idle, placeOfCreation);
    travelling = false;
    holding = Resource::Nothing;
    needed = Resource::Nothing;
    owner = placeOfCreation->owner;
    IMHere(placeOfCreation);
    godObject::global_server->sendPacketAll(Event(Event::Type::PAWN_APPEAR, id).getPacket());
}
void WorkerPawn::assignTask(const Task& toAssign) {
    currentTask = toAssign;
    toTake = false;
    toDrop = false;
    onTheWay.clear();
    currentInWay = 0;
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
        double signX = position.first - dest->position.first;
        double deltaX = fabs(position.first - dest->position.first);
        double signY = position.second - dest->position.second;
        double deltaY = fabs(position.second - dest->position.second);
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
            position.first += -signX * (deltaX / sqrt(wholeDelta)) * speed * deltaTime;
            position.second += -signY * (deltaY / sqrt(wholeDelta)) * speed * deltaTime;
        }
        if (signX * (position.first - dest->position.first) <= 1 && signY * (position.second - dest->position.second) <= 1) {
            IMHere(dest);
            ++currentInWay;
            if (currentInWay < onTheWay.size())
                godObject::global_server->sendPacketAll(Event(Event::Type::PAWN_MOVE, id, onTheWay[currentInWay]->position).getPacket());
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
                godObject::global_server->sendPacketAll(Event(Event::Type::BUILDING_REMOVE_RES, positionBuilding->id, needed).getPacket());
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

size_t WorkerPawn::deserialize(const std::vector<uint8_t>& data) { return deserializeSelf(data); }

std::vector<uint8_t> WorkerPawn::serializeSelf() const {
    std::vector<uint8_t> result = Pawn::serializeSelf();
    result.insert(result.begin(), DummyWorker);
    size_t size = sizeof(size_t) * 3 + sizeof(expertisesID) * expertises.size() + sizeof(ptr<Building>) * onTheWay.size();
    result.resize(result.size() + size);
    uint8_t* curr = result.data() + result.size();
    curr += copyVariable(curr, currentInWay);

    curr += copyVariable(curr, expertises.size());
    for (const auto& i: expertises) { curr += copyVariable(curr, i); }

    curr += copyVariable(curr, onTheWay.size());
    for (const auto& i: onTheWay) { curr += copyVariable(curr, i); }
    return result;
}


size_t WorkerPawn::deserializeSelf(const std::vector<uint8_t>& data) {
    size_t shift = Pawn::deserializeSelf(data);
    const uint8_t* curr = data.data() + shift;
    curr += initializeVariable(curr, currentInWay);

    size_t size;
    curr += initializeVariable(curr, size);
    for (size_t i = 0; i < size; ++i) {
        expertisesID tmp;
        curr += initializeVariable(curr, tmp);
    }

    curr += initializeVariable(curr, size);
    onTheWay.resize(size);
    for (size_t i = 0; i < size; ++i) { curr += initializeVariable(curr, onTheWay[i]); }
    return curr - data.data();
}

WorkerPawn::~WorkerPawn() {
#ifdef SERVER_SIDE
    owner->manager.cancelTask(currentTask, ptr<Pawn>(id));
    drop(positionBuilding);
    IMNotHere();
#endif
}
