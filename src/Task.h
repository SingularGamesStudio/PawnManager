#ifndef TASK_H
#define TASK_H
///misha does not believe in me  :(
#include "Resource.h"
#include "IDmanager.h"

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
    ptr<Building> destination;
    Resource object;
    ptr<Building> destination2;
    int returnID;

    bool avaliable(ptr<Player> owner);

    Task(TaskID taskid = TaskID::Idle, ptr<Building> building = ptr<Building>(), ptr<Building> building2 = ptr<Building>(), Resource resource = Resource::DummyNothing, int returnID = -1);
    ///TASK_REFORM(player -> pawn commands)
};
#endif //TASK_H