#include "Building.h"
#include "Pawn.h"
enum class TaskID{
    Build,
    Craft,
    Attack,
    PawnMove,
    PawnGet,
    Man,
};
class Task{
public:
    TaskID id;
    vector<Pawn> assignedTo;
    Building* destination;
    ///TASK_REFORM(player -> pawn commands)
};