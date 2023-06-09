#include "ServerMain.h"

#include <unistd.h>

#include <chrono>
#include <unordered_map>

#include "../Entities/Buildings/Building.h"
#include "../Entities/Buildings/BuildingRegisty.h"
#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/FighterPawn.h"
#include "../Entities/Pawns/WorkerPawn.h"
#include "../Recipes/CraftRecipe.h"
#include "../Recipes/WorkerRecipe.h"
#include "Event.h"
#include "IDmanager.h"
#include "Player.h"
#include "Resource.h"
#include "godobject.h"
#ifdef SERVER_SIDE
using std::string;

int fromString(const string& str) {
    int result = 0;
    for (char c: str) { result = 10 * result + c - '0'; }
    return result;
}

Position get_pos(int id) {
    int diag = 0;
    while (id > diag) {
        ++diag;
        id -= diag;
    }
    Position result(diag - id, id);
    result *= 500;
    return result;
}

ptr<CraftBuilding> makeHub(ptr<Player> player, int key) {
    Position hub_pos = get_pos(key);
    ptr<CraftBuilding> hub = makeptr<CraftBuilding>(hub_pos, player, 100.0);
    for (size_t i = 0; i < 10; ++i) hub->resources.insert(Resource::Wood);
    for (size_t i = 0; i < 10; ++i) hub->resources.insert(Resource::Stone);
    hub->resources.insert(Resource::Sword);
    player->hub = static_cast<ptr<Building>>(hub);
    hub->recipes.push_back(makeGrindRecipe({expertisesID::Nitwit}, {Resource::Wood}, 10.0));
    hub->recipes.push_back(makeGrindRecipe({expertisesID::Nitwit}, {Resource::Stone}, 10.0));

    auto wrecipe = new WorkerRecipe();
    wrecipe->outWorkers.emplace_back(std::vector<expertisesID>());
    for (int i = 0; i < 3; ++i) wrecipe->inResources.push_back(Resource::Wood);
    for (int i = 0; i < 3; ++i) wrecipe->inResources.push_back(Resource::Stone);
    wrecipe->duration = 5.0;
    hub->recipes.push_back(wrecipe);

    return hub;
}

bool GameServer::onConnection(std::shared_ptr<dlib::Connection> client) {
    ptr<Player> player = makeptr<Player>();
    player->manager.owner = player;
    ptr<CraftBuilding> hub = makeHub(player, IDs);
    {
        ptr<WorkerPawn> pawn = makeptr<WorkerPawn>();
        pawn->create(player->hub);
        pawn->expertises.insert(expertisesID::Nitwit);
        pawn->expertises.insert(expertisesID::Lumbering);
        pawn->expertises.insert(expertisesID::Mining);
        pawn->expertises.insert(expertisesID::Smithing);
        player->pawns.insert(pawn.dyn_cast<Pawn>());
    }
    {
        ptr<FighterPawn> pawn = FighterPawn::createFighterPawn(FighterPawnType::Monk, player->hub);
        player->pawns.insert(pawn.dyn_cast<Pawn>());
    }
    Event plA(Event::Type::PLAYER_APPEAR, player->id);
    Event hubA(Event::Type::BUILDING_APPEAR, hub->id);
    sendPacketAll(plA.getPacket());
    sendPacketAll(hubA.getPacket());
    for (auto i: player->pawns) {
        Event workerA(Event::Type::PAWN_APPEAR, i.id);
        sendPacketAll(workerA.getPacket());
    }
    players[client->getID()] = player;
    sendPacketClient(client, Event(Event::Type::SYNC_PULSE, players[client->getID()].id).getPacket());
    return true;
}

void GameServer::afterConnection(std::shared_ptr<dlib::Connection> client) {}

void GameServer::onDisconnection(std::shared_ptr<dlib::Connection> client) {}

void GameServer::onPacketReceive(std::shared_ptr<dlib::Connection> client, dlib::Packet p) {
    Event::Type type = static_cast<Event::Type>(p.data[0]);
    std::vector<uint8_t> data = p.data;
    data.erase(data.begin());
    if (type == Event::Type::PLAYER_ACTION) {
        int id = 0;
        std::memcpy(&id, data.data(), sizeof(int));
        Recipe* rec = nullptr;
        godObject::getRecipe(data.data() + sizeof(int), rec);
        players[client->getID()]->manager.startRecipe(rec, ptr<Building>(id));
        delete rec;
        return;
    } else if (type == Event::Type::ATTACK) {
        int id = 0;
        uint8_t* dota = data.data();
        dota += initializeVariable(dota, id);
        ptr<Building> toAttack(id);
        std::vector<std::pair<FighterPawnType, int>> fighters;
        dota += initializeVector(dota, fighters);
        players[client->getID()]->manager.attack(toAttack, fighters);
    }
}

void tickBuildings(ptr<Building> place, double deltaTime) {
    place->tick(deltaTime);
    for (ptr<Building> ch: place->children) { tickBuildings(ch, deltaTime); }
}

void GameServer::tick(double deltaTime) {
    for (auto pl: players) {
        pl.second->tick();
        for (ptr<Pawn> pw: pl.second->pawns) { pw->tick(deltaTime); }
        tickBuildings(pl.second->hub, deltaTime);
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
    godObject::global_server = &server;
    server.rnd = std::mt19937(42);
    double currTime = clock();
    BuildingRegisty::init();
    while (1) {
        double dTime = clock() - currTime;
        currTime += dTime;
        server.tick(dTime / CLOCKS_PER_SEC);
        server.respond();
        for (ptr<RequiresID> todel: godObject::global_server->suicideSquad) {
            if (todel.dyn_cast<Player>()) {
                auto pl = todel.dyn_cast<Player>();
                auto it = server.players.begin();
                while (it != server.players.end() && it->second != pl) ++it;
                server.players.erase(it);
                while (!pl->pawns.empty()) pl->pawns.begin()->del();
            }
            todel.del();
        }
        godObject::global_server->suicideSquad.clear();
    }
    server.stop();
}
#endif