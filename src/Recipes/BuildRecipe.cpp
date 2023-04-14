#include "BuildRecipe.h"

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
    cleanup(res);
}
#endif
Recipe* BuildRecipe::cloneSelf() {
    BuildRecipe* res = new BuildRecipe(pos, toBuild);
    return res;
}