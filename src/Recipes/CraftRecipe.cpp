#include "CraftRecipe.h"

#include <cstring>

#include "../Core/godobject.h"
#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/FighterPawn.h"

#ifdef SERVER_SIDE
void CraftRecipe::finish() {
    for (ptr<Pawn> p: procPawns) { p.del(); }

    for (FighterPawnType t: outFighters) {
        auto send = FighterPawn::createFighterPawn(t, place.dyn_cast<Building>()).dyn_cast<Pawn>();
        godObject::global_server->sendPacketAll(Event(Event::Type::PAWN_APPEAR, send.id).getPacket());
        send->currentTask = Task(TaskID::Protect, ptr<Entity>(), send->owner->hub);
        send->moveToBuilding(send->owner->hub);//TODO:костыль
    }

    for (Resource t: outResources) { place->addResource(t); }
    cleanup();
}
#endif

Recipe* CraftRecipe::cloneSelf() {
    CraftRecipe* res = new CraftRecipe();
    res->outResources = outResources;
    res->outFighters = outFighters;
    return res;
}

RecipeType CraftRecipe::getType() const { return RecipeType::CRAFT_RECIPE; }

std::vector<uint8_t> CraftRecipe::serialize() const { return serializeSelf(); }
size_t CraftRecipe::deserialize(const uint8_t* data) { return deserializeSelf(data); }

std::vector<uint8_t> CraftRecipe::serializeSelf() const {
    std::vector<uint8_t> result = Recipe::serializeSelf();
    size_t size = sizeof(size_t) * 2 + sizeof(uint8_t) * (outFighters.size() + outResources.size());
    result.resize(result.size() + size);
    uint8_t* curr = result.data() + result.size() - size;
    curr += copyVector(curr, outFighters);
    curr += copyVector(curr, outResources);
    return result;
}

size_t CraftRecipe::deserializeSelf(const uint8_t* data) {
    size_t shift = Recipe::deserializeSelf(data);
    const uint8_t* curr = data + shift;
    curr += initializeVector(curr, outFighters);
    curr += initializeVector(curr, outResources);
    return curr - data;
}