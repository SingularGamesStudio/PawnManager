#include "Building.h"
#include "Pawn.h"
enum class TaskID{
    Build,
    Craft,
    Attack,
    PawnMove,
    PawnGet,
    Man,
    Idle
};
class Task{
public:
    TaskID id;
    Pawn assignedTo;
    Building* destination;
    Task(TaskID taskid, Pawn pawn, Building* building): id(taskid), assignedTo(pawn), destination(building){

    }
    ///TASK_REFORM(player -> pawn commands)
};