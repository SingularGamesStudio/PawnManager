#include "BuildRecipe.h"
#include "../Entities/Buildings/Building.h"
#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/Pawn.h"

void BuildRecipe::finish() {
    for(ptr<Pawn> p :procPawns){p.del();}
    ptr<Building> res = ptr<Building>();
    if(!toBuild.available.empty()){
        res = static_cast<ptr<Building>>(makeptr<CraftBuilding>(place->position, place->owner, 100, place->radius, place->parent));
        static_cast<ptr<CraftBuilding>>(res)->recipes = toBuild.available;
    } else {
        res = makeptr<Building>(place->position, place->owner, 100, place->radius, place->parent);
    }
    res->children = place->children;
    for(ptr<Building> b:place->children) {
        b->parent = res;
    }
    for(auto it = res->parent->children.begin(); it!=res->parent->children.end(); it++){
        if((*it).id==place.id){
            res->parent->children.erase(it);
            break;
        }
    }
    res->parent->children.push_back(res);
    for(ptr<Pawn> p:res->pawns) {
        p->positionBuilding = res;
    }
    res->pawns = place->pawns;
    res->resources = place->resources;
    res->reservedResources = place->reservedResources;
    place.del();
    cleanup(res);
    std::cout << "aboba" << "\n";
}