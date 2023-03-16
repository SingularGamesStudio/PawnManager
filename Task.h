#include "Building.h"
#include "Pawn.h"
enum class TaskID {
    Build,
    Craft,
    Attack,
    Move,
    Get,
    Idle,
    Transport,
    BeProcessed
};
class Task{
public:
    TaskID id;
    Building* destination;
    Resource object;
    Building* destination2;
    Task(TaskID taskid, Building* building, Building* building2 = nullptr, Resource resource = Resource::DummyNothing): id(taskid), destination(building), destination2(building2), object(resource){

    }
    ///TASK_REFORM(player -> pawn commands)
};