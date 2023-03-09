#include "Building.h"
#include "Player.h"
enum class ResourceType {DummyOre, DummyIngot, DummyWeapon};
class Resource{
public:
    Player owner;
    ResourceType type;
    bool beingHolded;
    Building position;
};
