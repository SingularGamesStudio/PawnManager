#include "WorkerRecipe.h"

#include <cstring>

#include "../Core/godobject.h"
#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/FighterPawn.h"

#ifdef SERVER_SIDE
void WorkerRecipe::finish() {
    for (ptr<Pawn> p: procPawns) { p.del(); }

    for (ptr<WorkerPawn> p: workers) {
        for (expertisesID id: trainExpertises) { p->expertises.insert(id); }
        godObject::global_server->sendPacketAll(Event(Event::Type::UPDATE_EXPERTISES, p.id).getPacket());
    }

    for (auto exps: outWorkers) {
        ptr<WorkerPawn> pawn = makeptr<WorkerPawn>();
        pawn->create(place.dyn_cast<Building>());
        pawn->expertises.insert(expertisesID::Nitwit);
        for (auto exp: exps) { pawn->expertises.insert(exp); }
        place->owner->pawns.insert(pawn.dyn_cast<Pawn>());
        Event workerA(Event::Type::PAWN_APPEAR, pawn.id);
        godObject::global_server->sendPacketAll(workerA.getPacket());
    }

    cleanup();
}
#endif

Recipe* WorkerRecipe::cloneSelf() {
    WorkerRecipe* res = new WorkerRecipe();
    res->trainExpertises = trainExpertises;
    res->outWorkers = outWorkers;
    return res;
}

RecipeType WorkerRecipe::getType() const { return RecipeType::CRAFT_RECIPE; }

std::vector<uint8_t> WorkerRecipe::serialize() const { return serializeSelf(); }
size_t WorkerRecipe::deserialize(const uint8_t* data) { return deserializeSelf(data); }

std::vector<uint8_t> WorkerRecipe::serializeSelf() const {
    std::vector<uint8_t> result = Recipe::serializeSelf();
    size_t size = sizeof(size_t) * 2 + sizeof(uint8_t) * (outFighters.size() + outResources.size());
    result.resize(result.size() + size);
    uint8_t* curr = result.data() + result.size() - size;
    curr += copyVector(curr, outFighters);
    curr += copyVector(curr, outResources);
    return result;
}

size_t WorkerRecipe::deserializeSelf(const uint8_t* data) {
    size_t shift = Recipe::deserializeSelf(data);
    const uint8_t* curr = data + shift;
    curr += initializeVector(curr, outFighters);
    curr += initializeVector(curr, outResources);
    return curr - data;
}