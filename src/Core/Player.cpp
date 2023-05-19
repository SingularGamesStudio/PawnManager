#include "Player.h"

#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>

#include "../Entities/Buildings/Building.h"
#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/FighterPawn.h"
#include "../Entities/Pawns/WorkerPawn.h"
#include "../Recipes/BuildRecipe.h"
#include "Event.h"
#include "godobject.h"

#ifdef SERVER_SIDE

void dfs(ptr<Building> vertex, std::multiset<Resource>& resources) {
    for (Resource r: vertex->resources) {
        if (resources.contains(r)) resources.erase(resources.find(r));
    }

    for (ptr<Building> next: vertex->children) {
        if (resources.empty()) return;
        dfs(next, resources);
    }
}

ptr<Building> findResource(ptr<Building> vertex, Resource resource) {
    if (vertex->resources.contains(resource)) return vertex;
    for (ptr<Building> next: vertex->children) {
        ptr<Building> res = findResource(next, resource);
        if (res) return res;
    }
    return ptr<Building>();
}

bool Player::checkRecipe(Recipe* recipe) {
    std::multiset<Resource> resources;
    std::multiset<FighterPawnType> fighters;
    std::multiset<expertisesID> workers;
    for (Resource r: recipe->inResources) { resources.insert(r); }
    for (FighterPawnType t: recipe->inFighters) { fighters.insert(t); }
    for (expertisesID t: recipe->inWorkers) { workers.insert(t); }
    for (expertisesID t: recipe->reqWorkers) { workers.insert(t); }
    dfs(hub, resources);
    if (!resources.empty()) return false;
    for (auto it = pawns.rbegin(); it != pawns.rend(); ++it) {
        ptr<Pawn> pawn = *it;
        if (ptr<WorkerPawn> worker = pawn.dyn_cast<WorkerPawn>(); worker) {
            for (expertisesID e: worker->expertises) {//TODO:change to mincost
                if (workers.contains(e)) {
                    workers.erase(workers.find(e));
                    break;
                }
            }
        } else if (ptr<FighterPawn> fighter = pawn.dyn_cast<FighterPawn>(); fighter) {
            if (fighters.contains(fighter->getType())) fighters.erase(fighters.find(fighter->getType()));
        }
    }
    if (!fighters.empty() || !workers.empty()) return false;
    return true;
}

ptr<CraftBuilding> Player::placeBlueprint(Position pos, ptr<Building> parent, double r) {
    ptr<CraftBuilding> blueprint = makeptr<CraftBuilding>(pos, ptr<Player>(id), 100, r, parent);
    godObject::global_server->sendPacketAll(Event(Event::Type::BUILDING_APPEAR, blueprint.id).getPacket());
    parent->children.insert(blueprint.dyn_cast<Building>());
    return blueprint;
}

bool Player::TaskManager::startRecipe(Recipe* recipe, ptr<Building> where) {
    std::cerr << owner.id << std::endl;
    if (!owner->checkRecipe(recipe)) return false;
    if (dynamic_cast<BuildRecipe*>(recipe) != nullptr) {
        BuildRecipe* brecipe = dynamic_cast<BuildRecipe*>(recipe);
        where = (owner->placeBlueprint(brecipe->pos, where, brecipe->toBuild.radius)).dyn_cast<Building>();
    }
    work.insert(new PendingRecipe(recipe, where, 0));
    return true;
}

Player::TaskManager::PendingRecipe::PendingRecipe(Recipe* recipe, ptr<Building> place, int priority)
    : recipe(recipe->clone()),
      place(place),
      priority(priority),
      ID(IDmanager::newID()) {
    for (Resource r: this->recipe->inResources) { needResources.insert(r); }
    for (expertisesID e: this->recipe->reqWorkers) { needPawns.push_back(dynamic_cast<PawnReq*>(new WorkerReq(e))); }
    for (expertisesID e: this->recipe->inWorkers) { needPawns.push_back(dynamic_cast<PawnReq*>(new WorkerReq(e))); }
    for (FighterPawnType e: this->recipe->inFighters) { needPawns.push_back(dynamic_cast<PawnReq*>(new FighterReq(e))); }
}

void Player::tick() { manager.tick(); }

void Player::TaskManager::tick() {//TODO:rewrite to mincost
    std::vector<ptr<WorkerPawn>> haulers;
    for (ptr<Pawn> p: owner->pawns) {
        ptr<WorkerPawn> worker = p.dyn_cast<WorkerPawn>();
        if (worker && worker->currentTask.id == TaskID::Idle) { haulers.push_back(worker); }
    }
    std::vector<std::pair<PendingRecipe*, bool>> toClose;
    std::unordered_set<ptr<CraftBuilding>> busy;
    for (PendingRecipe* rec: work) {
        if (!rec->place) {
            std::cerr << "building destroyed, cancelling recipe"
                      << "\n";
            toClose.push_back({rec, false});
        } else if (!rec->needResources.empty()) {
            std::multiset<Resource> newNeed;
            for (Resource resource: rec->needResources) {
                ptr<Building> where = findResource(owner->hub, resource);
                if (!where) {
                    std::cerr << "no resource found, cancelling recipe"
                              << "\n";
                    toClose.push_back({rec, false});
                    break;
                }
                if (haulers.size() == 0) {
                    newNeed.insert(resource);
                } else {
                    haulers.back()->assignTask(Task(TaskID::Transport, where.dyn_cast<Entity>(), rec->place, resource, rec->ID));
                    haulers.pop_back();
                    rec->movedResources.insert(resource);
                }
            }
            rec->needResources = newNeed;
        } else if (rec->movedResources.empty()) {
            if (!rec->needPawns.empty()) {
                std::vector<std::pair<ptr<Pawn>, PawnReq*>> found;
                bool ok = true;
                for (PawnReq* p: rec->needPawns) {
                    ptr<Pawn> pawn = p->find(owner);
                    if (!pawn) {
                        ok = false;
                        break;
                    }
                    found.push_back({pawn, p});
                    pawn->currentTask.id = TaskID::Craft;
                }
                if (ok) {
                    for (auto z: found) {
                        ptr<Pawn> pawn = z.first;
                        PawnReq* p = z.second;
                        pawn->assignTask(Task(TaskID::BeProcessed, rec->place.dyn_cast<Entity>(), ptr<Building>(), Resource::Nothing, rec->ID));
                        rec->movedPawns.insert(pawn);
                        rec->backupNeeds[pawn] = p;
                    }
                    rec->needPawns.clear();
                } else {
                    for (auto z: found) { z.first->currentTask.id = TaskID::Idle; }
                    for (ptr<Pawn> pawn: rec->movedPawns) {
                        rec->needPawns.push_back(rec->backupNeeds[pawn]);
                        pawn->assignTask(Task(TaskID::Idle));
                    }
                    for (ptr<Pawn> pawn: rec->donePawns) {
                        rec->needPawns.push_back(rec->backupNeeds[pawn]);
                        pawn->assignTask(Task(TaskID::Idle));
                    }
                    rec->movedPawns.clear();
                    rec->donePawns.clear();
                    rec->backupNeeds.clear();
                }
            } else if (rec->movedPawns.empty()) {
                ptr<CraftBuilding> crafter = rec->place.dyn_cast<CraftBuilding>();
                if (crafter->current == nullptr && !busy.contains(crafter)) {
                    std::cout << "starting recipe" << std::endl;
                    toClose.push_back({rec, true});
                    busy.insert(crafter);
                }
            }
        }
    }

    for (auto pr: toClose) {
        PendingRecipe* cur = pr.first;
        if (pr.second) { cur->start(); }
        work.erase(cur);
        delete cur;
    }
}

void Player::TaskManager::PendingRecipe::start() {
    ptr<CraftBuilding> crafter = place.dyn_cast<CraftBuilding>();
    if (!place.dyn_cast<CraftBuilding>()) throw std::logic_error("requested recipe for building which could not craft");
    if (!crafter->assignRecipe(recipe)) {
        std::cerr << "requested recipe start in building by id " << place->id << ", but recipe requirements are not met" << std::endl;
    }
    donePawns.clear();
    doneResources.clear();
}

Player::TaskManager::PendingRecipe::~PendingRecipe() {
    for (PawnReq* p: needPawns) { delete p; }
    for (ptr<Pawn> p: movedPawns) {
        if (p) p->assignTask(Task(TaskID::Idle));
        delete backupNeeds[p];
    }
    for (ptr<Pawn> p: donePawns) {
        if (p) p->assignTask(Task(TaskID::Idle));
        delete backupNeeds[p];
    }
    if (place) {
        for (Resource r: doneResources) {
            if (place->reservedResources.contains(r)) {
                place->reservedResources.erase(place->reservedResources.find(r));
                place->resources.insert(r);
            } else
                std::cerr << "when deleting PendingRecipe, trying to unreserve resource, but it is not reserved";
        }
    }
    delete recipe;
}

void Player::TaskManager::attack(ptr<Building> what, std::vector<std::pair<FighterPawnType, int>> fighters) {
    for (auto p: fighters) {
        FighterPawnType type = p.first;
        int cnt = p.second;
        for (ptr<Pawn> pawn: owner->pawns) {
            if (pawn.dyn_cast<FighterPawn>()) {
                ptr<FighterPawn> fighter = pawn.dyn_cast<FighterPawn>();
                if (fighter->getType() == type && fighter->currentTask.id == TaskID::Idle) {
                    cnt--;
                    if (what->owner == owner) {
                        fighter->assignTask(Task(TaskID::Protect, ptr<Entity>(), what));
                    } else {
                        fighter->assignTask(Task(TaskID::Attack, what.dyn_cast<Entity>()));
                    }
                    if (cnt == 0) break;
                }
            }
        }
        if(cnt>0)
            std::cerr << cnt << " fighters not found" << std::endl;
    }
}

void Player::TaskManager::cancelTask(Task task, ptr<Pawn> pawn) {
    PendingRecipe* pr = nullptr;
    for (PendingRecipe* t: work) {
        if (t->ID == task.returnID) {
            pr = t;
            break;
        }
    }
    if (pr == nullptr) return;
    switch (task.id) {
        case TaskID::Transport:
            pr->movedResources.erase(pr->movedResources.find(task.object));
            pr->needResources.insert(task.object);
            break;
        case TaskID::BeProcessed:
            pr->needPawns.push_back(pr->backupNeeds[pawn]);
            pr->movedPawns.erase(pawn);
            pr->backupNeeds.erase(pawn);
            break;
        default:
            break;
    }
}

void Player::TaskManager::finishTask(Task task, ptr<Pawn> pawn) {
    PendingRecipe* pr = nullptr;
    for (PendingRecipe* t: work) {
        if (t->ID == task.returnID) {
            pr = t;
            break;
        }
    }
    if (pr == nullptr) return;

    switch (task.id) {
        case TaskID::Transport:
            pr->movedResources.erase(pr->movedResources.find(task.object));
            pr->doneResources.insert(task.object);
            task.destination2->resources.erase(task.destination2->resources.find(task.object));
            task.destination2->reservedResources.insert(task.object);
            break;
        case TaskID::BeProcessed:
            pr->movedPawns.erase(pawn);
            pr->donePawns.push_back(pawn);
            break;
        default:
            std::cout << "pawn is not ready :(\n";//YOU ARE NOT PREPARED
            break;
    }
}

ptr<Pawn> Player::TaskManager::FighterReq::find(ptr<Player> owner) {
    for (auto it = owner->pawns.rbegin(); it != owner->pawns.rend(); ++it) {
        auto p = *it;
        ptr<FighterPawn> f = p.dyn_cast<FighterPawn>();
        if (f && f->getType() == type && f->currentTask.id == TaskID::Idle) { return p; }
    }
    return ptr<Pawn>();
}

ptr<Pawn> Player::TaskManager::WorkerReq::find(ptr<Player> owner) {
    for (auto it = owner->pawns.rbegin(); it != owner->pawns.rend(); ++it) {
        auto p = *it;
        ptr<WorkerPawn> w = p.dyn_cast<WorkerPawn>();
        if (w && w->expertises.contains(expertise) && w->currentTask.id == TaskID::Idle) { return p; }
    }
    return ptr<Pawn>();
}

void Player::attack(ptr<Building> what) {
    for (ptr<Pawn> p: pawns) {
        if (p.dyn_cast<FighterPawn>()) {
            ptr<FighterPawn> f = p.dyn_cast<FighterPawn>();
            f->assignTask(Task(TaskID::Attack, what.dyn_cast<Entity>()));
        }
    }
}

Player::~Player() {
    hub.del();
    while (!pawns.empty()) (*(pawns.begin())).del();
}
#endif

#ifdef CLIENT_SIDE
void Player::localAttack(ptr<Building> what, std::vector<std::pair<FighterPawnType, int>> who) {
    Event attack(Event::Type::ATTACK, what.id, who);
    godObject::local_server->send(attack.getPacket());
}
void Player::localStart(Recipe* recipe, ptr<Building> where) {
    Event recipeStarted(Event::Type::PLAYER_ACTION, recipe, where.id);
    godObject::local_server->send(recipeStarted.getPacket());
}
#endif

std::vector<uint8_t> Player::serialize() const { return serializeSelf(); }
size_t Player::deserialize(const uint8_t* data) { return deserializeSelf(data); }

std::vector<uint8_t> Player::serializeSelf() const {
    std::vector<uint8_t> result = RequiresID::serializeSelf();
    size_t size = sizeof(int) + sizeof(ptr<Building>) + sizeof(size_t) + sizeof(ptr<Pawn>) * pawns.size();
    result.resize(result.size() + size);
    uint8_t* curr = result.data() + result.size() - size;
    curr += copyVariable(curr, id);
    curr += copyVariable(curr, hub);
    curr += copySet(curr, pawns);
    return result;
}

size_t Player::deserializeSelf(const uint8_t* data) {
    size_t shift = RequiresID::deserializeSelf(data);
    const uint8_t* curr = data + shift;
    curr += initializeVariable(curr, id);
    curr += initializeVariable(curr, hub);
    size_t size;
    curr += initializeVariable(curr, size);
    for (size_t i = 0; i < size; ++i) {
        ptr<Pawn> tmp;
        curr += initializeVariable(curr, tmp);
        pawns.insert(tmp);
    }
    return curr - data;
}

void buildingDFS(ptr<Building> v, std::vector<ptr<Building>>& result) {
    result.push_back(v);
    for (auto u: v->children) { buildingDFS(u, result); }
}

std::vector<ptr<Building>> Player::get_buildings() const {
    std::vector<ptr<Building>> result;
    buildingDFS(hub, result);
    return result;
}