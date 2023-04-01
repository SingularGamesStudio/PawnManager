#include "BuildRecipe.h"
#include "../Entities/Buildings/Building.h"
#include "../Entities/Buildings/CraftBuilding.h"

void BuildRecipe::finish() {
    for(Pawn* p :procPawns){delete p;}
    Building* res = nullptr;
    if(!toBuild.available.empty()){
        res = new CraftBuilding(place->position, place->owner, 100, place->radius, place->parent);
        dynamic_cast<CraftBuilding*>(res)->recipes = toBuild.available;
    } else {
        res = new Building(place->position, place->owner, 100, place->radius, place->parent);
    }
    res->children = place->children;
    for(Building* b:place->children) {
        b->parent = res;
    }
    for(auto it = res->parent->children.begin(); it!=res->parent->children.end(); it++){
        if(*it==place){
            res->parent->children.erase(it);
            break;
        }
    }
    res->parent->children.push_back(res);
    delete place;
    cleanup(res);
}