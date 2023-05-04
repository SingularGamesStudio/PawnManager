#include "Event.h"

#include <stdexcept>

#include "Entities/Buildings/Building.h"
#include "Entities/Pawns/Pawn.h"
#include "Entities/ResourceEntity.h"
#include "IDmanager.h"
#include "Player.h"
#include "Recipes/Recipe.h"

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
        curr_data = pl->serialize();// todo
    } else if (t == Event::Type::RESOURCE_ENTITY_APPEAR) {
        ptr<ResourceEntity> pl(id);
        curr_data = pl->serialize();// todo
    } else {
        tmp.resize(sizeof(t) + sizeof(id));
        std::memcpy(tmp.data() + sizeof(t), &id, sizeof(id));
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
    std::memcpy(tmp.data(), &t, sizeof(t));
    std::memcpy(tmp.data() + sizeof(t), &id, sizeof(id));
    std::memcpy(tmp.data() + sizeof(t) + sizeof(id), &res, sizeof(res));
    p << tmp;
}

Event::Event(Event::Type t, int id, std::pair<double, double> pos, double time) {
    if (t != Event::Type::PAWN_MOVE) throw std::invalid_argument("Trying to make event with wrong type");
    std::vector<uint8_t> tmp(sizeof(t) + sizeof(id) + sizeof(pos) + sizeof(time));
    std::memcpy(tmp.data(), &t, sizeof(t));
    std::memcpy(tmp.data() + sizeof(t), &id, sizeof(id));
    std::memcpy(tmp.data() + sizeof(t) + sizeof(id), &pos, sizeof(pos));
    std::memcpy(tmp.data() + sizeof(t) + sizeof(id) + sizeof(pos), &time, sizeof(time));
    p << tmp;
}

Event::Event(Type t, Recipe *recipe, int id) {
    if (t != Event::Type::PLAYER_ACTION) throw std::invalid_argument("Trying to make event with wrong type");
    std::vector<uint8_t> tmp(sizeof(t) + sizeof(id));
    std::memcpy(tmp.data(), &t, sizeof(t));
    std::memcpy(tmp.data() + sizeof(t), &id, sizeof(id));
    std::vector<uint8_t> curr_data = recipe->serialize();
    std::copy(curr_data.begin(), curr_data.end(), std::back_inserter(tmp));
    p << tmp;
}

dlib::Packet Event::getPacket() {
    dlib::Packet result = p;
    result.prepare();
    return result;
}