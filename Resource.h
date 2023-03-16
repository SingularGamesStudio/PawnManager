#include "Building.h"
#include "Player.h"

enum class ResourceType {DummyOre, DummyIngot, DummyWeapon};
class Resource{
public:

    Resource(Player owner, ResourceType type):owner(owner), type(type), position(nullptr){}

    Player owner;
    ResourceType type;
    Building position;
};
