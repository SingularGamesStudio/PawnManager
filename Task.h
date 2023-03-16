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
    Task(TaskID taskid, Building* building): id(taskid), destination(building){

    }
    ///TASK_REFORM(player -> pawn commands)
};