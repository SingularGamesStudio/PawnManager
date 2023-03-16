#ifndef TASK_H
#define TASK_H
///misha does not believe in me  :(
#include "Resource.h"
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

class Building;

class Task{
public:
    TaskID id;
    Building* destination;
    Resource object;
    Building* destination2;
    Task(TaskID taskid, Building* building, Building* building2 = nullptr, Resource resource = Resource::DummyNothing);
    ///TASK_REFORM(player -> pawn commands)
};
#endif //TASK_H