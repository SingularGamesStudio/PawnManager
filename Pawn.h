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
    virtual void moveTo(std::pair<double, double> destination);
    void create(Building* placeOfCreation) {
        used = false;
        currentTask = Task(TaskID::Idle, placeOfCreation);
        travelling = false;
        holding = nullptr;
        destination = placeOfCreation;
        inside = placeOfCreation;
    }
    void drop(Building* in, std::pair<double, double> pos = { 0,0 }) {
        if (in != nullptr) {
            in->addResource(holding);
            ~Pawn();
            return;
        }
        ResourceEntity(holding, pos);
        ~Pawn();
    }
    void destroy() {
        if (inside != nullptr && holding != nullptr) {
            (*holding).drop(inside);
        }
        else if (holding != nullptr) {
            (*holding).drop(position);
        }
        ///кинь игроку факт, что хотелось бы другого чувака на таску
        ~Pawn();
    }
    void IMNotHere(Building* from) {
        from.Pawns.erase(from.Pawns.find(this));
    }
    void IMHere(Building* to) {
        to.Pawns.push_back(this);
    }
    virtual void moveToBuilding(Building* toMove);
    virtual void assignTask(const Task& toAssign);
    void beIngridient() {
        assignTask(TaskID::BeProcessed);
    }
    void stopBeingIngridient() {
        assignTask(TaskID::Idle);
    }
};