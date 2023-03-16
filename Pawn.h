#include <utility>
#include "Resource.h"
#include "Task.h"
#include "Entity.h"
class Building;
class Pawn : public Entity {
public:
    Building* inside = nullptr;
    Resource holding;
    Building* destination;
    Task CurrentTask;
    bool travelling;
    static const int ticksPerSecond = 100'000'000;
    void drop(Building* in, std::pair<double, double> pos = { 0,0 });
    void destroy();
    void IMNotHere(Building* from);
    void IMHere(Building* to);
    virtual void moveToBuilding(Building* toMove);
    virtual void assignTask(const Task& toAssign);
    void beIngridient();
    void stopBeingIngridient();

};