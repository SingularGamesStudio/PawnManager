#include "Player.h"
#include "Entities/Buildings/Building.h"
#include "Entities/Pawns/WorkerPawn.h"
#include "Entities/Pawns/FighterPawn.h"
#include "Recipes/BuildRecipe.h"
#include "Entities/Buildings/CraftBuilding.h"
#include <set>
#include <vector>

void dfs(Building* vertex, std::multiset<Resource>& resources){
    for(Resource r:vertex->resources) {
        if(resources.contains(r))
            resources.erase(resources.find(r));
    }

    for(Building* next:vertex->children) {
        if (resources.empty())
            return;
        dfs(next, resources);
    }
}

Building* findResource(Building* vertex, Resource resource){
    if(vertex->resources.contains(resource))
        return  vertex;
    for(Building* next:vertex->children) {
        Building* res = findResource(next, resource);
        if(res) return res;
    }
    return nullptr;
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
    for(Pawn* pawn:pawns){
        if(WorkerPawn* worker = dynamic_cast<WorkerPawn*>(pawn); worker!=nullptr) {
            for(expertisesID e:worker->expertises){//TODO:change to mincost
                if(workers.contains(e)){
                    workers.erase(workers.find(e));
                    break;
                }
            }
        } else if(FighterPawn* fighter = dynamic_cast<FighterPawn*>(pawn); fighter!=nullptr) {
            if(fighters.contains(fighter->getType()))
                fighters.erase(fighters.find(fighter->getType()));
        }
    }
    if(!fighters.empty() || !workers.empty())
        return false;
    return true;
}

CraftBuilding* Player::placeBlueprint(std::pair<double, double> pos, Building *parent, double r) {
    CraftBuilding* blueprint = new CraftBuilding(pos, this, 100, r, parent);
    parent->children.push_back(dynamic_cast<Building*>(blueprint));
    return blueprint;
}

bool Player::TaskManager::startRecipe(Recipe* recipe, Building* where) {
    if(!owner->checkRecipe(recipe))
        return false;
    if(dynamic_cast<BuildRecipe*>(recipe)!= nullptr) {
        BuildRecipe* brecipe = dynamic_cast<BuildRecipe*>(recipe);
        where = owner->placeBlueprint(brecipe->pos, where, brecipe->toBuild.radius);
    }
    work.insert(new PendingRecipe(recipe, where, 0));
    return true;
}

Player::TaskManager::PendingRecipe::PendingRecipe(Recipe *recipe, Building *place, int priority): recipe(recipe), place(place), priority(priority), ID(IDmanager::newID()) {
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
    std::vector<WorkerPawn*> haulers;
    for(Pawn* p:owner->pawns) {
        WorkerPawn* worker = dynamic_cast<WorkerPawn*>(p);
        if(worker!=nullptr && worker->currentTask.id==TaskID::Idle) {
            haulers.push_back(worker);
        }
    }
    std::vector<std::pair<PendingRecipe*, bool>> toClose;
    for(PendingRecipe* rec : work){
        if(!rec->needResources.empty()){
            for(Resource resource:rec->needResources){
                Building* where = findResource(owner->hub, resource);
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
                    Pawn* pawn = p->find(owner);
                    if(pawn== nullptr) {
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
    CraftBuilding* crafter = dynamic_cast<CraftBuilding*>(place);
    if(crafter== nullptr)
        throw std::logic_error("requested recipe for building which could not craft");
    if(!crafter->assignRecipe(recipe)) {
        std::cerr << "requested recipe start in building by id " << place->id << ", but recipe requirements are not met\n";
    }
    donePawns.clear();
    doneResources.clear();
}

Player::TaskManager::PendingRecipe::~PendingRecipe() {
    for(Pawn* p :movedPawns) {
        p->assignTask(Task(TaskID::Idle));
    }
    for(Pawn* p :donePawns) {
        p->assignTask(Task(TaskID::Idle));
    }
    for(Resource r:doneResources) {
        if(place->reservedResources.contains(r))
            place->reservedResources.erase(place->reservedResources.find(r));
        else
            std::cerr << "when deleting PendingRecipe, trying to unreserve resource, but it is not reserved";
    }
}

void Player::TaskManager::cancelTask(Task task, Pawn* pawn) {
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

void Player::TaskManager::finishTask(Task task, Pawn* pawn) {
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