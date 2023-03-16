#include "Building.h"

class Pawn : public Entity{
public:
    Building* inside = nullptr;

    Building* destination;
    virtual void moveTo(Building* destination);
    virtual void getResource(Resource* toGet);
};