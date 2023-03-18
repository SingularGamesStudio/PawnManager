#include "WorkerPawn.h"
#include "../Buildings/Building.h"
#include <cmath>
void WorkerPawn::create(Building* placeOfCreation) {
    currentTask = Task(TaskID::Idle, placeOfCreation);
    travelling = false;
    holding = Resource::DummyNothing;
    owner = placeOfCreation->owner;
    destination = placeOfCreation;
    inside = placeOfCreation;
}
void WorkerPawn::assignInnerTask(Task toAssign) {
    currentTask = toAssign;
    switch (toAssign.id) {
    case TaskID::Get:
        takeResourceFromBuilding(toAssign.destination, toAssign.object);
    case TaskID::Transport:
        takeResourceFromBuilding(toAssign.destination, toAssign.object);
        moveResourceTo(toAssign.destination2);
    case TaskID::Move:
        moveToBuilding(toAssign.destination);
    case TaskID::BeProcessed:
        moveToBuilding(toAssign.destination);
    default:
        throw("Unexpected WorkerPawn TaskID: ", toAssign.id);
    }
}
void WorkerPawn::tick() {
    if (currentInWay < onTheWay.size()){
        Building* dest = onTheWay[currentInWay];
        position.first+= (dest->position.first - dest->position.first) / ticksPerSecond;
        position.second+= (dest->position.first - dest->position.first) / ticksPerSecond;
        if (fabs(position.first - dest->position.first) < 1e-6 && fabs(position.second - dest->position.second) < 1e-6){
            position = dest->position;
            ++currentInWay;
        }
    }
    else
    travelling = false;
}
void WorkerPawn::moveToBuilding(Building* dest) {
    travelling = true;
    std::unordered_map<Building*, std::vector<Building*> > visited;
    std::queue<Building*> q;
    destination = dest;
    q.push(positionBuilding);
    while (!q.empty()) {
        Building* currentB = q.front();
        q.pop();
        std::vector<Building*> way = visited[currentB];
        for (Building* toGo : currentB->children)
        {
            if (!visited[toGo].empty())
                continue;
            visited[toGo] = way;
            visited[toGo].push_back(toGo);
            q.push(toGo);
        }
        if (!visited[currentB->parent].empty())
            continue;
        visited[currentB->parent] = way;
        visited[currentB->parent].push_back(currentB->parent);
        q.push(currentB->parent);
    }
    for (Building* v : visited[dest])
    {
        onTheWay.push_back(v);
    }
}
void WorkerPawn::moveToNeighbour(Building* dest) {
    double currentTime = 0;
    double timePerMove = sqrt(abs(position.first - dest->position.first) * abs(position.first - dest->position.first) +
        abs(position.second - dest->position.second) * abs(position.second - dest->position.second));
    while (currentTime < timePerMove)
    {
        currentTime += timePerMove / ticksPerSecond;
        position.first = currentTime / ticksPerSecond * (*positionBuilding).position.first
            + (1 - currentTime / ticksPerSecond) * (*positionBuilding).position.first;
        position.second = currentTime / ticksPerSecond * (*dest).position.second
            + (1 - currentTime / ticksPerSecond) * (*dest).position.second;
    }
    positionBuilding = dest;
    position = dest->position;
}
void WorkerPawn::takeResourceFromBuilding(Building * dest, Resource res) {
    moveToBuilding(dest);
    dest->resources.erase(dest->resources.find(res));
    holding = res;
}
void WorkerPawn::moveResourceTo(Building * dest) {
    moveToBuilding(dest);
    drop(inside,position);
}
