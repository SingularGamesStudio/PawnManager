#include "Player.h"
#include "Entities/Buildings/Building.h"
#include <set>
#include <vector>

void dfs(Building* vertex, std::multiset<Resource>& resources){
    if(resources.size()<vertex->resources.size()){
        std::vector<Resource> toDelete;
        for(Resource r:resources) {
            if(vertex->resources.contains(r)){
                toDelete.push_back(r);
            }
        }
        for(Resource r:toDelete){
            resources.erase(resources.find(r));
        }
    } else {
        for(Resource r:vertex->resources) {
            if(resources.contains(r))
                resources.erase(resources.find(r));
        }
    }
    for(Building* next:vertex->children) {
        if (resources.empty())
            return;
        dfs(next, resources);
    }
}

bool Player::checkRecipe(Recipe recipe) {
    std::multiset<Resource> resources;
    for(Resource r:recipe.inResources){resources.insert(r);}
    dfs(hub, resources);
    if(!resources.empty())
        return false;

}

bool Player::startRecipe(Recipe recipe, Building* where) {

}