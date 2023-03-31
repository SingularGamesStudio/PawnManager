#ifndef TASK_H
#define TASK_H
///misha does not believe in me  :(
#include "Resource.h"

class Player;

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
    int returnID;

    bool avaliable(Player* owner);

    Task(TaskID taskid = TaskID::Idle, Building* building = nullptr, Building* building2 = nullptr, Resource resource = Resource::DummyNothing, int returnID = -1);
    ///TASK_REFORM(player -> pawn commands)
};
#endif //TASK_H