#include "Task.h"

#include "Player.h"

Task::Task(TaskID taskid, ptr<Building> building, ptr<Building> building2, Resource resource, int returnID)
    : id(taskid),
      destination(building),
      destination2(building2),
      object(resource),
      returnID(returnID) {}

bool Task::avaliable(ptr<Player> owner) {
    ///TODO
    return false;
}