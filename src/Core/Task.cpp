#include "Task.h"

#include "Player.h"

Task::Task(TaskID taskid, ptr<Entity> dest, ptr<Building> building2, Resource resource, int returnID)
    : id(taskid),
      destination(dest),
      destination2(building2),
      object(resource),
      returnID(returnID) {}

bool Task::avaliable(ptr<Player> owner) {
    ///TODO
    return false;
}