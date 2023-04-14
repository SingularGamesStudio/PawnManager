#ifndef EVENT_H
#define EVENT_H
#include "net/packet.h"

class Building;
class Pawn;
class Recipe;
enum class Resource : uint8_t;

class Event {
private:
    dlib::Packet p;

public:
    enum class Type : uint8_t {
        BUILDING_APPEAR,
        BUILDING_DISAPPEAR,
        BUILDING_ADD_RES,
        BUILDING_REMOVE_RES,
        PAWN_APPEAR,
        PAWN_DISAPPEAR,
        PAWN_MOVE,
        PAWN_TAKE_RES,
        PAWN_LET_RES,
        PLAYER_APPEAR,
        PLAYER_DISAPPEAR,
        RESOURCE_ENTITY_APPEAR,
        RESOURCE_ENTITY_DISAPPEAR,
        PLAYER_ACTION
    };
    Event() = delete;
    Event(Type t, int id);
    Event(Type t, int id, Resource res);
    Event(Type t, int id, std::pair<double, double> pos, double time);
    Event(Type t, Recipe* recipe, int id);
    dlib::Packet getPacket();
};
#endif