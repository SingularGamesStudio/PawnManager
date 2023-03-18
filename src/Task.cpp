#include "Task.h"
#include "Player.h"

Task::Task(TaskID taskid, Building* building, Building* building2, Resource resource): id(taskid), destination(building), destination2(building2), object(resource){

}

bool Task::avaliable(Player* owner) {

}