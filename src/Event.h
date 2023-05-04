#include "net/packet.h"

class Building;
class Pawn;
enum class Resource;

class Event {
private:
    Packet p;

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
        PAWN_LET_RES
    };
    Event() = delete;
    Event(Type t, int id);
    Event(Type t, int id, Resource res);
    Event(Type t, int id, std::pair<double, double> pos);
    dlib::Packet getPacket();
};
