#include "Building.h"
#include "Task.h"
#include <utility>
class Pawn : public Entity{
public:
    Building* inside = nullptr;
    Resource holding;
    Building* destination;
    Task CurrentTask;
    bool travelling;
    static const int ticksPerSecond = 100'000'000;
    void drop(Building* in, std::pair<double, double> pos = { 0,0 }) {
        if (in != nullptr && holding != Resource::DummyNothing) {
            in->addResource(holding);
            holding = Resource::DummyNothing;
            return;
        } 
        if(holding != Resource::DummyNothing)
            ResourceEntity(holding, pos);
    }
    void destroy() {
        if (inside != nullptr && holding != Resorce::DummyNothing) {
            (*holding).drop(inside);
        }
        else if (holding != Resource::DummyNothing) {
            (*holding).drop(position);
        }
        ///task not done... god object taskmanager not happy :(
        ~Pawn();
    }
    void IMNotHere(Building* from) {
        if(from != nullptr)
            from->removePawn(this);
    }
    void IMHere(Building* to) {
        if(to != nullptr)
            to->addPawn(this);
    }
    virtual void moveToBuilding(Building* toMove) = 0;
    virtual void assignTask(const Task& toAssign) = 0;
    void beIngridient() {
        assignTask(TaskID::BeProcessed);
    }
    void stopBeingIngridient() {
        assignTask(TaskID::Idle);
    }

};