#include "Building.h"

class Pawn : public Entity{
public:
    Building* inside = nullptr;
    bool used;
    Resource* holding;
    Building* destination;
    Task CurrentTask;
    bool travelling;
    virtual void moveTo(Building* destination);
    virtual void getResource(Resource* toGet);
    void beIngridient() {
        used = true;
    }
    void stopBeingIngridient() {
        used = false;
    }
};