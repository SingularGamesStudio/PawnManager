#include "Player.h"
#include "Entities/Buildings/Building.h"
#include "Entities/Pawns/WorkerPawn.h"
#include "Entities/Pawns/FighterPawn.h"
#include "Recipes/BuildRecipe.h"
#include "Entities/Buildings/CraftBuilding.h"
#include <set>
#include <vector>

void dfs(ptr<Building> vertex, std::multiset<Resource>& resources){
    for(Resource r:vertex->resources) {
        if(resources.contains(r))
            resources.erase(resources.find(r));
    }

    for(ptr<Building> next:vertex->children) {
        if (resources.empty())
            return;
        dfs(next, resources);
    }
}

ptr<Building> findResource(ptr<Building> vertex, Resource resource){
    if(vertex->resources.contains(resource))
        return  vertex;
    for(ptr<Building> next:vertex->children) {
        ptr<Building> res = findResource(next, resource);
        if(res) return res;
    }
    return ptr<Building>();
}

bool Player::checkRecipe(Recipe* recipe) {
    std::multiset<Resource> resources;
    std::multiset<FighterPawnType> fighters;
    std::multiset<expertisesID> workers;
    for(Resource r:recipe->inResources){resources.insert(r);}
    for(FighterPawnType t:recipe->inFighters){fighters.insert(t);}
    for(expertisesID t:recipe->inWorkers){workers.insert(t);}
    for(expertisesID t:recipe->reqWorkers){workers.insert(t);}
    dfs(hub, resources);
    if(!resources.empty())
        return false;
    for(ptr<Pawn> pawn:pawns){
        if(ptr<WorkerPawn> worker = static_cast<ptr<WorkerPawn>>(pawn); worker) {
            for(expertisesID e:worker->expertises){//TODO:change to mincost
                if(workers.contains(e)){
                    workers.erase(workers.find(e));
                    break;
                }
            }
        } else if(ptr<FighterPawn> fighter = static_cast<ptr<FighterPawn>>(pawn); fighter) {
            if(fighters.contains(fighter->getType()))
                fighters.erase(fighters.find(fighter->getType()));
        }
    }
    if(!fighters.empty() || !workers.empty())
        return false;
    return true;
}

ptr<CraftBuilding> Player::placeBlueprint(std::pair<double, double> pos, ptr<Building>parent, double r) {
    ptr<CraftBuilding> blueprint = makeptr<CraftBuilding>(pos, ptr<Player>(id), 100, r, parent);
    parent->children.push_back(static_cast<ptr<Building>>(blueprint));
    return blueprint;
}

bool Player::TaskManager::startRecipe(Recipe* recipe, ptr<Building> where) {
    if(!owner->checkRecipe(recipe))
        return false;
    if(dynamic_cast<BuildRecipe*>(recipe)!= nullptr) {
        BuildRecipe* brecipe = dynamic_cast<BuildRecipe*>(recipe);
        where = static_cast<ptr<Building>>(owner->placeBlueprint(brecipe->pos, where, brecipe->toBuild.radius));
    }
    work.insert(new PendingRecipe(recipe, where, 0));
    return true;
}

Player::TaskManager::PendingRecipe::PendingRecipe(Recipe *recipe, ptr<Building>place, int priority): recipe(recipe), place(place), priority(priority), ID(IDmanager::newID()) {
    for(Resource r :recipe->inResources) {
        needResources.insert(r);
    }
    for(expertisesID e:recipe->reqWorkers){
        needPawns.push_back(dynamic_cast<PawnReq*>(new WorkerReq(e)));
    }
    for(expertisesID e:recipe->inWorkers){
        needPawns.push_back(dynamic_cast<PawnReq*>(new WorkerReq(e)));
    }
    for(FighterPawnType e:recipe->inFighters){
        needPawns.push_back(dynamic_cast<PawnReq*>(new FighterReq(e)));
    }
}

void Player::tick() {
    manager.tick();
}

void Player::TaskManager::tick() {//TODO:rewrite to mincost
    std::vector<ptr<WorkerPawn>> haulers;
    for(ptr<Pawn> p:owner->pawns) {
        ptr<WorkerPawn> worker = static_cast<ptr<WorkerPawn>>(p);
        if(worker && worker->currentTask.id==TaskID::Idle) {
            haulers.push_back(worker);
        }
    }
    std::vector<std::pair<PendingRecipe*, bool>> toClose;
    for(PendingRecipe* rec : work){
        if(!rec->needResources.empty()){
            for(Resource resource:rec->needResources){
                ptr<Building> where = findResource(owner->hub, resource);
                if(!where){
                    toClose.push_back({rec, false});
                    break;
                }
                haulers.back()->assignTask(Task(TaskID::Transport, where, rec->place, resource, rec->ID));
                haulers.pop_back();
                rec->movedResources.insert(resource);
            }
            rec->needResources.clear();
        } else if(rec->movedResources.empty()) {
            if(!rec->needPawns.empty()) {
                for(PawnReq* p :rec->needPawns){
                    ptr<Pawn> pawn = p->find(owner);
                    if(pawn) {
                        toClose.push_back({rec, false});
                        break;
                    }
                    pawn->moveToBuilding(rec->place);//TODO:remake this when the method is implemented
                    rec->movedPawns.push_back(pawn);
                }
                rec->needPawns.clear();
            } else if(rec->movedPawns.empty()) {
                toClose.push_back({rec, true});
            }
        }
    }

    for(auto pr:toClose) {
        PendingRecipe* cur = pr.first;
        if(pr.second){
            cur->start();
        }
        work.erase(cur);
        delete cur;
    }
}

void Player::TaskManager::PendingRecipe::start() {
    ptr<CraftBuilding> crafter = static_cast<ptr<CraftBuilding>>(place);
    if(!place.dyn_cast<CraftBuilding>())
        throw std::logic_error("requested recipe for building which could not craft");
    if(!crafter->assignRecipe(recipe)) {
        std::cerr << "requested recipe start in building by id " << place->id << ", but recipe requirements are not met\n";
    }
    donePawns.clear();
    doneResources.clear();
}

Player::TaskManager::PendingRecipe::~PendingRecipe() {
    for(ptr<Pawn> p :movedPawns) {
        p->assignTask(Task(TaskID::Idle));
    }
    for(ptr<Pawn> p :donePawns) {
        p->assignTask(Task(TaskID::Idle));
    }
    for(Resource r:doneResources) {
        if(place->reservedResources.contains(r))
            place->reservedResources.erase(place->reservedResources.find(r));
        else
            std::cerr << "when deleting PendingRecipe, trying to unreserve resource, but it is not reserved";
    }
}

void Player::TaskManager::cancelTask(Task task, ptr<Pawn> pawn) {
    PendingRecipe* pr = nullptr;
    for(PendingRecipe* t : work){
        if(t->ID==task.returnID){
            pr = t;
            break;
        }
    }
    if(pr== nullptr)
        return;
    switch (task.id) {
        case TaskID::Transport:
            pr->movedResources.erase(pr->movedResources.find(task.object));
            pr->needResources.insert(task.object);
            break;
        case TaskID::BeProcessed:
            //TODO
            break;
        default:
            break;
    }
}

void Player::TaskManager::finishTask(Task task, ptr<Pawn> pawn) {
    PendingRecipe* pr = nullptr;
    for(PendingRecipe* t : work){
        if(t->ID==task.returnID){
            pr = t;
            break;
        }
    }
    if(pr== nullptr)
        return;

    switch (task.id) {
        case TaskID::Transport:
            pr->movedResources.erase(pr->movedResources.find(task.object));
            break;
        case TaskID::BeProcessed:
            //TODO
            break;
        default:
            break;
    }
}