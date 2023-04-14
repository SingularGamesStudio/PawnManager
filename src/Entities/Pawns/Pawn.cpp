#include "Pawn.h"

#include <cstring>

#include "../../Event.h"
#include "../../Player.h"
#include "../../Resource.h"
#include "../../godobject.h"
#include "../Buildings/Building.h"
#include "../ResourceEntity.h"
#ifdef SERVER_SIDE

void Pawn::drop(ptr<Building> in, std::pair<double, double> pos) {
    if (in && holding != Resource::Nothing) {
        in->addResource(holding);
        godObject::global_server->sendPacketAll(Event(Event::Type::BUILDING_ADD_RES, in->id, holding).getPacket());
        holding = Resource::Nothing;
        godObject::global_server->sendPacketAll(Event(Event::Type::PAWN_LET_RES, id).getPacket());
        return;
    }
    if (holding != Resource::Nothing) {
        // TODO godObject::global_server->sendPacketAll(Event(Event::Type::RESOURCE_ENTITY_ADD, holding).getPacket());
        makeptr<ResourceEntity>(holding, pos);
    }
}
void Pawn::IMNotHere() {
    if (positionBuilding) {
        positionBuilding->removePawn(ptr<Pawn>(id));
        positionBuilding = ptr<Building>();
    }
}
void Pawn::IMHere(ptr<Building> to) {
    if (to) {

        IMNotHere();
        to->addPawn(ptr<Pawn>(id));
        positionBuilding = to;
        position = positionBuilding->position;
    }
}
void Pawn::stopBeingIngridient() { assignTask(Task(TaskID::Idle, positionBuilding)); }
void Pawn::moveToBuilding(ptr<Building> toMove) { throw("How did we get here?"); }
//void Pawn::assignTask(const Task& toAssign) = 0;

void Pawn::tick(double deltaTime) {}
#endif
std::vector<uint8_t> Pawn::serialize() const { return serializeSelf(); }

size_t Pawn::deserialize(const std::vector<uint8_t>& data) { return deserializeSelf(data); }

std::vector<uint8_t> Pawn::serializeSelf() const {
    std::vector<uint8_t> result = Entity::serializeSelf();
    size_t size = sizeof(Resource) * 2 + sizeof(ptr<Building>) + sizeof(Task) + sizeof(bool) * 3 + sizeof(double);
    result.resize(result.size() + size);
    uint8_t* curr = result.data() + result.size() - size;
    curr += copyVariable(curr, holding);
    curr += copyVariable(curr, needed);
    curr += copyVariable(curr, positionBuilding);
    curr += copyVariable(curr, currentTask);
    curr += copyVariable(curr, speed);
    curr += copyVariable(curr, travelling);
    curr += copyVariable(curr, toDrop);
    curr += copyVariable(curr, toTake);
    return result;
}


size_t Pawn::deserializeSelf(const std::vector<uint8_t>& data) {
    size_t shift = Entity::deserializeSelf(data);
    const uint8_t* curr = data.data() + shift;
    curr += initializeVariable(curr, holding);
    curr += initializeVariable(curr, needed);
    curr += initializeVariable(curr, positionBuilding);
    curr += initializeVariable(curr, currentTask);
    curr += initializeVariable(curr, speed);
    curr += initializeVariable(curr, travelling);
    curr += initializeVariable(curr, toDrop);
    curr += initializeVariable(curr, toTake);
    return curr - data.data();
}

Pawn::~Pawn() {
    owner->pawns.erase(ptr<Pawn>(id));
    godObject::global_server->sendPacketAll(Event(Event::Type::PAWN_DISAPPEAR, id).getPacket());
}
