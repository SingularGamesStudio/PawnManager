#include "Building.h"
#include "Player.h"

enum class ResourceType {DummyOre, DummyIngot, DummyWeapon};
class Resource{
public:

    Resource(Player owner, ResourceType type):owner(owner), type(type), position(nullptr){}

    Player owner;
    ResourceType type;
    Building position;
    void drop(Building* inHere, std::pair<double, double> pos = { 0,0 }) {
        if (inHere == nullptr) {
            ResourceEntity(this, pos);
            return;
        }
        inHere->resources.push_back((*this));
    }
};
