#ifndef EVENT_H
#define EVENT_H
#include "../Entities/Entity.h"
#include "../net/packet.h"

class Building;
class Pawn;
class Recipe;
enum class FighterPawnType : uint8_t;
enum class Resource : uint8_t;

class Event {
private:
    dlib::Packet p = dlib::Packet(dlib::Packet::Type::RAW_MESSAGE);

public:
    enum class Type : uint8_t {
        BUILDING_APPEAR,          //0
        BUILDING_DISAPPEAR,       //1
        BUILDING_ADD_RES,         //2
        BUILDING_REMOVE_RES,      //3
        PAWN_APPEAR,              //4
        PAWN_DISAPPEAR,           //5
        PAWN_MOVE,                //6
        PAWN_TAKE_RES,            //7
        PAWN_LET_RES,             //8
        PLAYER_APPEAR,            //9
        PLAYER_DISAPPEAR,         //10
        RESOURCE_ENTITY_APPEAR,   //11
        RESOURCE_ENTITY_DISAPPEAR,//12
        PLAYER_ACTION,            //13
        SYNC_PULSE,               //14
        UPDATE_RESOURCES,         //15
        ATTACK,                   //16
        UPDATE_EXPERTISES,        //17
    };
    Event() = delete;
    Event(Type t, int id);
    Event(Type t, int id, Resource res);
    Event(Type t, int id, int id_dest, double time);
    Event(Type t, Recipe* recipe, int id);
    Event(Type t, int id, std::vector<std::pair<FighterPawnType, int>> what);
    dlib::Packet getPacket();
};
#endif