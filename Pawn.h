#include "Building.h"
#include "Task.h"
#include <utility>
class Pawn : public Entity{
public:
    Building* inside = nullptr;
    bool used;
    Resource* holding;
    Building* destination;
    Task CurrentTask;
    bool travelling;
    virtual void moveTo(std::pair<double, double> destination);
    void create(Building* placeOfCreation) {
        used = false;
        currentTask = Task(TaskID::Idle, placeOfCreation);
        travelling = false;
        holding = nullptr;
        destination = placeOfCreation;
        inside = placeOfCreation;
    }
    void destroy() {
        if (inside != nullptr && holding != nullptr) {
            (*holding).drop(inside);
        }
        else if (holding != nullptr) {
            (*holding).drop(position);
        }
        ~Pawn();

    }
    virtual void moveToResource(Resource* toGet);
    virtual void moveToBuilding(Building* toMove);
    virtual void assignTask(const Task& toAssign);
    void getResource(Resource* toGet) {
        moveToResource(toGet);
        holding = toGet;
    }
    void beIngridient() {
        used = true;
    }
    void stopBeingIngridient() {
        used = false;
    }
};