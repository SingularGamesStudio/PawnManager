#ifndef TASK_H
#define TASK_H
///misha does not believe in me  :(
#include "IDmanager.h"
#include "Resource.h"

class Player;

enum class TaskID { Build, Craft, Attack, Move, Get, Idle, Transport, BeProcessed, DropResource, Protect };

class Building;

class Task {
public:
    TaskID id;
    ptr<Entity> destination;
    Resource object;
    ptr<Building> destination2;
    int returnID;

    bool avaliable(ptr<Player> owner);

    Task(TaskID taskid = TaskID::Idle, ptr<Entity> dest = ptr<Entity>(), ptr<Building> building2 = ptr<Building>(),
         Resource resource = Resource::Nothing, int returnID = -1);
    ///TASK_REFORM(player -> pawn commands)
};
#endif//TASK_H