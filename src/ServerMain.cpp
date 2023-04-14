#include "ServerMain.h"

#include <unistd.h>

#include <chrono>
#include <unordered_map>

#include "Entities/Buildings/Building.h"
#include "Entities/Buildings/BuildingRegisty.h"
#include "Entities/Buildings/CraftBuilding.h"
#include "Entities/Pawns/FighterPawn.h"
#include "Entities/Pawns/WorkerPawn.h"
#include "Player.h"
#include "Recipes/CraftRecipe.h"

using std::string;

int fromString(const string& str) {
    int result = 0;
    for (char c: str) { result = 10 * result + c - '0'; }
    return result;
}

bool GameServer::onConnection(std::shared_ptr<dlib::Connection> client) {
    ptr<Player> player = makeptr<Player>();
    CraftRecipe* recipe;
    ptr<CraftBuilding> crafter;
    player->manager.owner = player;
    BuildingRegisty::init();
    std::pair<double, double> pos = std::make_pair(rnd() % 300, rnd() % 300);
    std::pair<double, double> pos1 = std::make_pair(pos.first + rnd() % 30, pos.second + rnd() % 30);
    player->hub = makeptr<Building>(pos, player, 100);
    for (int i = 0; i < 30; i++) { player->hub->addResource(Resource::DummyOre); }
    for (int i = 0; i < 5; i++) {
        ptr<WorkerPawn> pawn = makeptr<WorkerPawn>();
        pawn->create(player->hub);
        pawn->expertises.insert(expertisesID::DummySmeltery);
        player->pawns.insert(pawn.dyn_cast<Pawn>());
    }
    crafter = makeptr<CraftBuilding>(pos1, player, 100);
    player->hub->children.insert(crafter.dyn_cast<Building>());
    crafter->parent = player->hub;
    recipe = new CraftRecipe();
    recipe->inResources.push_back(Resource::DummyOre);
    recipe->reqWorkers.push_back(expertisesID::DummySmeltery);
    recipe->outResources.push_back(Resource::DummyIngot);
    recipe->duration = 5;
    crafter->recipes.push_back(recipe);
    recipe = new CraftRecipe();
    recipe->inResources.push_back(Resource::DummyOre);
    recipe->outFighters.push_back(FighterPawnType::DummySwordsman);
    recipe->duration = 2;
    crafter->recipes.push_back(recipe);
    players[client->getID()] = player;
}

void GameServer::onDisconnection(std::shared_ptr<dlib::Connection> client) {}

void GameServer::onPacketReceive(std::shared_ptr<dlib::Connection> client, dlib::Packet p) {}

void tickBuildings(ptr<Building> place, double deltaTime) {
    place->tick(deltaTime);
    for (ptr<Building> ch: place->children) { tickBuildings(ch, deltaTime); }
}
std::mt19937 rnd(42);
void GameServer::tick(double deltaTime) {
    for (auto p: players) {
        p.second->tick();
        for (ptr<Pawn> p: p.second->pawns) { p->tick(deltaTime); }
        tickBuildings(p.second->hub, deltaTime);
    }
}

int main(int argc, char** argv) {
    std::unordered_map<string, int> params;
    params["port"] = 57179;
    params["env"] = false;
    std::string prev = "";
    for (int i = 1; i < argc; ++i) {
        if (!prev.empty()) {
            params[prev] = fromString(string(argv[i]));
            prev = "";
        } else
            prev = string(argv[i]).substr(2);
    }
    GameServer server(params["port"]);
    server.start();
    server.rnd = std::mt19937(42);
    auto currTime = std::chrono::steady_clock::now();
    while (1) {
        auto dTime = std::chrono::steady_clock::now() - currTime;
        currTime += dTime;
        server.tick(dTime.count());
        server.respond();
    }
    server.stop();
}