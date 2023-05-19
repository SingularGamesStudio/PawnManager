#include "BuildRecipe.h"

#include "../Core/godobject.h"
#include "../Entities/Buildings/Building.h"
#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/Pawn.h"
#ifdef SERVER_SIDE
void BuildRecipe::finish() {
    for (ptr<Pawn> p: procPawns) { p.del(); }
    ptr<Building> res = ptr<Building>();
    if (!toBuild.available.empty()) {
        res = (makeptr<CraftBuilding>(place->position, place->owner, 100, place->radius, place->parent)).dyn_cast<Building>();
        res.dyn_cast<CraftBuilding>()->recipes = toBuild.available;
    } else {
        res = makeptr<Building>(place->position, place->owner, 100, place->radius, place->parent);
    }
    res->children = place->children;
    res->pawns = place->pawns;
    res->resources = place->resources;
    res->reservedResources = place->reservedResources;
    place->pawns.clear();
    place->resources.clear();
    place->reservedResources.clear();
    place->children.clear();
    place->parent = ptr<Building>();
    place->current = nullptr;
    int newid = place.id;
    place.del();
    IDmanager::set(newid, res.pointer());
    IDmanager::set(res.id, nullptr);
    res.id = newid;
    res->id = newid;
    godObject::global_server->sendPacketAll(Event(Event::Type::BUILDING_APPEAR, res.id).getPacket());
    cleanup(res);
}
#endif
Recipe* BuildRecipe::cloneSelf() {
    BuildRecipe* res = new BuildRecipe(pos, toBuild);
    return res;
}

RecipeType BuildRecipe::getType() const { return RecipeType::BUILD_RECIPE; }

std::vector<uint8_t> BuildRecipe::serialize() const { return serializeSelf(); }
size_t BuildRecipe::deserialize(const uint8_t* data) { return deserializeSelf(data); }

std::vector<uint8_t> BuildRecipe::serializeSelf() const {
    std::vector<uint8_t> result = Recipe::serializeSelf();
    size_t size = sizeof(double) * 2 + sizeof(toBuild.index);
    result.resize(result.size() + size);
    uint8_t* curr = result.data() + result.size() - size;
    curr += copyVariable(curr, pos);
    curr += copyVariable(curr, toBuild.index);
    return result;
}

size_t BuildRecipe::deserializeSelf(const uint8_t* data) {
    size_t shift = Recipe::deserializeSelf(data);
    const uint8_t* curr = data + shift;
    curr += initializeVariable(curr, pos);
    int ind;
    curr += initializeVariable(curr, ind);
    toBuild = BuildingRegisty::database[ind]->toBuild;
    return curr - data;
}