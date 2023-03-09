//
// Created by admin on 09.03.2023.
//

#ifndef PAWNMANAGER_RESOURCE_H
#define PAWNMANAGER_RESOURCE_H
enum class ResourceType {DummyOre, DummyIngot, DummyWeapon};
class Resource{
public:
    Player owner;
    ResourceType type;
    bool beingHolded;
    Building position;
};
#endif //PAWNMANAGER_RESOURCE_H
