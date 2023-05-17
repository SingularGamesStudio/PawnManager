#include "Event.h"

#include <stdexcept>

#include "../Entities/Buildings/Building.h"
#include "../Entities/Pawns/Pawn.h"
#include "../Entities/ResourceEntity.h"
#include "IDmanager.h"
#include "Player.h"
#include "../Recipes/Recipe.h"
#include "godobject.h"

Event::Event(Event::Type t, int id) {
    std::set<Event::Type> acceptable_events;
    acceptable_events.insert(Event::Type::BUILDING_APPEAR);
    acceptable_events.insert(Event::Type::BUILDING_DISAPPEAR);
    acceptable_events.insert(Event::Type::PAWN_APPEAR);
    acceptable_events.insert(Event::Type::PAWN_DISAPPEAR);
    acceptable_events.insert(Event::Type::PAWN_LET_RES);
    acceptable_events.insert(Event::Type::PLAYER_APPEAR);
    acceptable_events.insert(Event::Type::PLAYER_DISAPPEAR);
    acceptable_events.insert(Event::Type::RESOURCE_ENTITY_APPEAR);
    acceptable_events.insert(Event::Type::RESOURCE_ENTITY_DISAPPEAR);
    acceptable_events.insert(Event::Type::ATTACK);
#ifdef SERVER_SIDE
    acceptable_events.insert(Event::Type::SYNC_PULSE);
#endif
    acceptable_events.insert(Event::Type::UPDATE_RESOURCES);
    if (!acceptable_events.contains(t)) throw std::invalid_argument("Trying to make event with wrong type");
    std::vector<uint8_t> tmp(sizeof(t));
    std::memcpy(tmp.data(), &t, sizeof(t));
    std::vector<uint8_t> curr_data;
    if (t == Event::Type::BUILDING_APPEAR) {
        ptr<Building> bu(id);
        curr_data = bu->serialize();
    } else if (t == Event::Type::PAWN_APPEAR) {
        ptr<Pawn> pw(id);
        curr_data = pw->serialize();
    } else if (t == Event::Type::PLAYER_APPEAR) {
        ptr<Player> pl(id);
        curr_data = pl->serialize();
    } else if (t == Event::Type::RESOURCE_ENTITY_APPEAR) {
        ptr<ResourceEntity> pl(id);
        curr_data = pl->serialize();// todo
    } else if (t == Event::Type::UPDATE_RESOURCES) {
        ptr<Building> bu(id);
        curr_data = bu->serializeResources();
    }
#ifdef SERVER_SIDE
    else if(t == Event::Type::SYNC_PULSE) {
        std::vector<ptr<Building>> all_buildings;
        for(auto i : godObject::global_server->players) {
            if(!i.second)
                continue;
            std::vector<ptr<Building>> curr_buildings = i.second->get_buildings();
            std::copy(curr_buildings.begin(), curr_buildings.end(),
                      std::back_inserter(all_buildings));
        }
        curr_data.resize(sizeof(size_t));
        copyVariable(curr_data.data(), all_buildings.size());
        std::copy(curr_data.begin(), curr_data.end(), std::back_inserter(tmp));
        for(auto i : all_buildings) {
            curr_data = i->serialize();
            std::copy(curr_data.begin(), curr_data.end(), std::back_inserter(tmp));
        }

        std::vector<ptr<Pawn>> all_pawns;
        for(auto i : godObject::global_server->players) {
            if(!i.second)
                continue;
            std::copy(i.second->pawns.begin(), i.second->pawns.end(), std::back_inserter(all_pawns));
        }
        curr_data.resize(sizeof(size_t));
        copyVariable(curr_data.data(), all_pawns.size());
        std::copy(curr_data.begin(), curr_data.end(), std::back_inserter(tmp));
        for(auto i : all_pawns) {
            curr_data = i->serialize();
            std::copy(curr_data.begin(), curr_data.end(), std::back_inserter(tmp));
        }


        std::vector<ptr<Player>> all_players;
        for(auto i : godObject::global_server->players) {
            if(!i.second)
                continue;
            all_players.push_back(i.second);
        }
        curr_data.resize(sizeof(size_t));
        copyVariable(curr_data.data(), all_players.size());
        std::copy(curr_data.begin(), curr_data.end(), std::back_inserter(tmp));
        for(auto i : all_players) {
            curr_data = i->serialize();
            std::copy(curr_data.begin(), curr_data.end(), std::back_inserter(tmp));
        }

        curr_data.resize(sizeof(int));
        copyVariable(curr_data.data(), id);
    }
#endif
    else {
        tmp.resize(sizeof(t) + sizeof(id));
        copyVariable(tmp.data() + sizeof(t), id);
    }
    std::copy(curr_data.begin(), curr_data.end(), std::back_inserter(tmp));
    p << tmp;
}

Event::Event(Event::Type t, int id, Resource res) {
    std::set<Event::Type> acceptable_events;
    acceptable_events.insert(Event::Type::BUILDING_ADD_RES);
    acceptable_events.insert(Event::Type::BUILDING_REMOVE_RES);
    acceptable_events.insert(Event::Type::PAWN_TAKE_RES);
    if (!acceptable_events.contains(t)) throw std::invalid_argument("Trying to make event with wrong type");
    std::vector<uint8_t> tmp(sizeof(t) + sizeof(id) + sizeof(res));
    uint8_t* data = tmp.data();
    data += copyVariable(data, t);
    data += copyVariable(data, id);
    data += copyVariable(data, res);
    p << tmp;
}

Event::Event(Event::Type t, int id, int id_dest, double velocity) {
    if (t != Event::Type::PAWN_MOVE) throw std::invalid_argument("Trying to make event with wrong type");
    std::vector<uint8_t> tmp(sizeof(t) + sizeof(int) + sizeof(int) + sizeof(velocity));
    uint8_t* data = tmp.data();
    data += copyVariable(data, t);
    data += copyVariable(data, id);
    data += copyVariable(data, id_dest);
    data += copyVariable(data, velocity);
    p << tmp;
}

Event::Event(Type t, Recipe *recipe, int id) {
    if (t != Event::Type::PLAYER_ACTION) throw std::invalid_argument("Trying to make event with wrong type");
    std::vector<uint8_t> tmp(sizeof(t) + sizeof(id));
    uint8_t* data = tmp.data();
    data += copyVariable(data, t);
    data += copyVariable(data, id);
    std::vector<uint8_t> curr_data = recipe->serialize();
    std::copy(curr_data.begin(), curr_data.end(), std::back_inserter(tmp));
    p << tmp;
}

dlib::Packet Event::getPacket() {
    dlib::Packet result = p;
    result.prepare();
    return result;
}