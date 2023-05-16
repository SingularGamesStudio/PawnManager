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
        int pawnType = data[0];
        data.erase(data.begin());
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
        pawn->deserialize(data.data());
        IDmanager::set(pawn->id, dynamic_cast<RequiresID*>(pawn));
    } else if (type == Event::Type::RESOURCE_ENTITY_APPEAR) {
        ResourceEntity* rEntity = new ResourceEntity();
        rEntity->deserialize(data.data());
        IDmanager::set(rEntity->id, dynamic_cast<RequiresID*>(rEntity));
    } else if (type == Event::Type::BUILDING_APPEAR) {
        BuildingType buildingType = static_cast<BuildingType>(data[0]);
        data.erase(data.begin());
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
        building->deserialize(data.data());
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
        std::memcpy(&id, data.data(), sizeof(int));
        Position pos;
        std::memcpy(&pos, data.data() + sizeof(int), sizeof(pos));
        double time;
        std::memcpy(&time, data.data() + sizeof(int) + sizeof(pos), sizeof(time));
        ptr<Entity>(id)->startMoveToPos(pos, time);
    } else if (type == Event::Type::UPDATE_RESOURCES) {
        int id = 0;
        std::memcpy(&id, data.data(), sizeof(int));
        ptr<Building> bu(id);
        bu->updateResources(data.data() + sizeof(int));
    } else {
        int id = 0;
        std::memcpy(&id, data.data(), sizeof(int));
        ptr<RequiresID>(id).del();
    }
}

void LocalController::init(std::string host, uint16_t port) {
    connect(host, port);
    awaitPacket([this](const dlib::Packet& pack) {
        Player* p = new Player();
        p->deserialize(pack.data.data() + 1);
        IDmanager::set(p->id, dynamic_cast<RequiresID*>(p));
        mainPlayer = ptr<Player>(p->id);
        players.insert(ptr<Player>(p->id));
    });
}

#endif