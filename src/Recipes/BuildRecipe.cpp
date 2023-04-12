#include "BuildRecipe.h"

#include "../Entities/Buildings/Building.h"
#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/Pawn.h"

void BuildRecipe::finish() {
    for (ptr<Pawn> p: procPawns) { p.del(); }
    ptr<Building> res = ptr<Building>();
    if (!toBuild.available.empty()) {
        res = static_cast<ptr<Building>>(makeptr<CraftBuilding>(place->position, place->owner, 100, place->radius, place->parent));
        static_cast<ptr<CraftBuilding>>(res)->recipes = toBuild.available;
    } else {
        res = makeptr<Building>(place->position, place->owner, 100, place->radius, place->parent);
    }
    res->children = place->children;
    res->pawns = place->pawns;
    res->resources = place->resources;
    res->reservedResources = place->reservedResources;
    int newid = place.id;
    place.del();
    IDmanager::set(newid, res.pointer());
    IDmanager::set(res.id, nullptr);
    res.id = newid;
    cleanup(res);
}