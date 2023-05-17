//
// Created by ME on 14.04.2023.
//
#ifdef CLIENT_SIDE

#include "LocalController.h"

#include "../Entities/Buildings/Building.h"
#include "../Entities/Buildings/BuildingRegisty.h"
#include "../Entities/Buildings/CraftBuilding.h"
#include "../Entities/Pawns/FighterPawn.h"
#include "../Entities/Pawns/WorkerPawn.h"

std::pair<Building*, size_t> getBuilding(uint8_t* data) {
    BuildingType buildingType = static_cast<BuildingType>(data[0]);
    ++data;
    Building* building = nullptr;
    switch (buildingType) {
        case BuildingType::BASE_BUILDING:
            building = new Building();
            break;
        case BuildingType::CRAFT_BUILDING:
            building = new CraftBuilding();
            break;
        default:
            break;
    }
    return {building, building->deserialize(data) + 1};
}

std::pair<Pawn*, size_t> getPawn(uint8_t* data) {
    int pawnType = data[0];
    ++data;
    Pawn* pawn = nullptr;
    switch (pawnType) {
        case DummyWorker:
            pawn = new WorkerPawn();
            break;
        case static_cast<uint8_t>(FighterPawnType::Monk):
            pawn = new Monk();
            break;
        case static_cast<uint8_t>(FighterPawnType::Swordsman):
            pawn = new Swordsman();
            break;
        default:
            break;
    }
    return {pawn, pawn->deserialize(data) + 1};
}

void LocalController::onPacketReceive(const dlib::Packet& p) {
    Event::Type type = static_cast<Event::Type>(p.data[0]);
    std::cerr << "Packet received " << int(type) << std::endl;
    std::vector<uint8_t> data = p.data;
    data.erase(data.begin());
    if (type == Event::Type::PLAYER_APPEAR) {
        Player* p = new Player(-1);
        p->deserialize(data.data());
        IDmanager::set(p->id, dynamic_cast<RequiresID*>(p));
        players.insert(ptr<Player>(p->id));
    } else if (type == Event::Type::PLAYER_DISAPPEAR) {
        int id = 0;
        std::memcpy(&id, data.data(), sizeof(int));
        if (id == mainPlayer.id) {
            std::cerr << "YOU DIED" << std::endl;
            exit(0);
        }
        ptr<Player>(id).del();
    } else if (type == Event::Type::PAWN_APPEAR) {
        Pawn* pawn = getPawn(data.data()).first;
        pawn->owner->pawns.insert(ptr<Pawn>(pawn->id));
        IDmanager::set(pawn->id, dynamic_cast<RequiresID*>(pawn));
    } else if (type == Event::Type::RESOURCE_ENTITY_APPEAR) {
        ResourceEntity* rEntity = new ResourceEntity();
        rEntity->deserialize(data.data());
        IDmanager::set(rEntity->id, dynamic_cast<RequiresID*>(rEntity));
    } else if (type == Event::Type::BUILDING_APPEAR) {
        Building* building = getBuilding(data.data()).first;
        if (building->parent) building->parent->children.insert(ptr<Building>(building->id));
        IDmanager::set(building->id, dynamic_cast<RequiresID*>(building));
    } else if (type == Event::Type::BUILDING_ADD_RES) {
        int id = 0;
        std::memcpy(&id, data.data(), sizeof(int));
        uint8_t resource;
        std::memcpy(&resource, data.data() + sizeof(int), sizeof(uint8_t));
        ptr<Building>(id)->addResource(static_cast<Resource>(resource));
    } else if (type == Event::Type::BUILDING_REMOVE_RES) {
        int id = 0;
        std::memcpy(&id, data.data(), sizeof(int));
        uint8_t resource;
        std::memcpy(&resource, data.data() + sizeof(int), sizeof(uint8_t));
        ptr<Building>(id)->removeResource(static_cast<Resource>(resource));
    } else if (type == Event::Type::PAWN_LET_RES) {
        int id = 0;
        std::memcpy(&id, data.data(), sizeof(int));
        ptr<Pawn>(id)->holding = Resource::Nothing;
    } else if (type == Event::Type::PAWN_TAKE_RES) {
        int id = 0;
        std::memcpy(&id, data.data(), sizeof(int));
        uint8_t resource;
        std::memcpy(&resource, data.data() + sizeof(int), sizeof(uint8_t));
        ptr<Pawn>(id)->holding = static_cast<Resource>(resource);
    } else if (type == Event::Type::PAWN_MOVE) {
        int id = 0;
        uint8_t* dota = data.data();
        dota += initializeVariable(dota, id);
        int id_where;
        dota += initializeVariable(dota, id_where);
        double velocity;
        dota += initializeVariable(dota, velocity);
        ptr<Entity>(id)->startMoveToPos(ptr<Entity>(id_where)->position, velocity);
        /// TODO: change to move to entity cmd
    } else if (type == Event::Type::UPDATE_RESOURCES) {
        int id = 0;
        std::memcpy(&id, data.data(), sizeof(int));
        ptr<Building> bu(id);
        bu->updateResources(data.data() + sizeof(int));
    } else if (type == Event::Type::UPDATE_EXPERTISES) {
        int id = 0;
        std::memcpy(&id, data.data(), sizeof(int));
        ptr<WorkerPawn> bu(id);
        bu->updateExpertises(data.data() + sizeof(int));
    } else if (type == Event::Type::SYNC_PULSE) {
        uint8_t* dota = data.data();

        size_t buildingCnt;
        dota += initializeVariable(dota, buildingCnt);
        for (size_t i = 0; i < buildingCnt; ++i) {
            auto res = getBuilding(dota);
            dota += res.second;
            Building* building = res.first;
            IDmanager::set(building->id, dynamic_cast<RequiresID*>(building));
        }

        size_t pawnCnt;
        dota += initializeVariable(dota, pawnCnt);
        for (size_t i = 0; i < pawnCnt; ++i) {
            auto res = getPawn(dota);
            dota += res.second;
            Pawn* pawn = res.first;
            IDmanager::set(pawn->id, dynamic_cast<RequiresID*>(pawn));
        }

        size_t playerCnt;
        dota += initializeVariable(dota, playerCnt);
        for (size_t i = 0; i < playerCnt; ++i) {
            Player* p = new Player(-1);
            dota += p->deserialize(dota);
            IDmanager::set(p->id, dynamic_cast<RequiresID*>(p));
            players.insert(ptr<Player>(p->id));
        }

        initializeVariable(dota, mainPlayer);

    } else {
        int id = 0;
        std::memcpy(&id, data.data(), sizeof(int));
        ptr<RequiresID>(id).del();
    }
}

void LocalController::init(std::string host, uint16_t port) {
    connect(host, port);
    awaitPacket([this](const dlib::Packet& pack) { onPacketReceive(pack); });
}

#endif