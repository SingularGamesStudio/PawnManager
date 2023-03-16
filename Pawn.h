#include "Building.h"
#include "Task.h"
#include <utility>
class Pawn : public Entity{
public:
    Building* inside = nullptr;
    bool used;
    Resource holding;
    Building* destination;
    Task CurrentTask;
    bool travelling;
    static const int ticksPerSecond = 100'000'000;
    void drop(Building* in, std::pair<double, double> pos = { 0,0 }) {
        if (in != nullptr) {
            in->addResource(holding);
            holding = Resource::DummyNothing;
            return;
        }
        ResourceEntity(holding, pos);

    }
    void destroy() {
        if (inside != nullptr && holding != Resorce::DummyNothing) {
            (*holding).drop(inside);
        }
        else if (holding != Resource::DummyNothing) {
            (*holding).drop(position);
        }
        ///кинь игроку факт, что хотелось бы другого чувака на таску
        ~Pawn();
    }
    void IMNotHere(Building* from) {
        from->removePawn(this);
    }
    void IMHere(Building* to) {
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