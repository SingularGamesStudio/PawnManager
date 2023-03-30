#include "WorkerPawn.h"
#include "../Buildings/Building.h"
#include <cmath>
#include <iostream>
void WorkerPawn::create(Building* placeOfCreation) {
    currentTask = Task(TaskID::Idle, placeOfCreation);
    travelling = false;
    holding = Resource::DummyNothing;
    needed = Resource::DummyNothing;
    owner = placeOfCreation->owner;
    //destination = placeOfCreation;
    IMHere(placeOfCreation);
}
void WorkerPawn::assignTask(const Task& toAssign) {
    currentTask = toAssign;
    //std::cout << "task " << (int)toAssign.id << "\n";
    switch (toAssign.id) {
    case TaskID::Get:
        moveToBuilding(toAssign.destination);
        needed = toAssign.object;
        toTake = true;
        break;
    case TaskID::Transport:
        moveToBuilding(toAssign.destination);
        //moveToBuilding(toAssign.destination2);
        toTake = true;
        needed = toAssign.object;
        break;
    case TaskID::Move:
        moveToBuilding(toAssign.destination);
        break;
    case TaskID::BeProcessed:
        moveToBuilding(toAssign.destination);
        break;
    case TaskID::Idle:
        break;
    default:
        throw("Unexpected WorkerPawn TaskID: ", toAssign.id);
    }
}
void WorkerPawn::tick(double deltaTime) {
    if (currentInWay < onTheWay.size()){
        Building* dest = onTheWay[currentInWay];
        double signX = position.first - dest->position.first;
        double deltaX = fabs(position.first - dest->position.first);
        double signY = position.second - dest->position.second;
        double deltaY = fabs(position.second - dest->position.second);
        double wholeDelta = deltaX * deltaX + deltaY * deltaY;
        if (signX < -1e-2){
            signX = -1;
        } else if (signX > 1e-2){
            signX = 1;
        } else
            signX = 0;
        if (signY < -1e-2){
            signY = -1;
        } else if (signY > 1e-2){
            signY = 1;
        } else
            signY = 0;
        if (wholeDelta > 1e-7) {
            position.first += -signX * (deltaX / sqrt(wholeDelta)) * speed * deltaTime;
            position.second += -signY * (deltaY / sqrt(wholeDelta)) * speed * deltaTime;
        }
        std::cerr<< position.first <<' '<< position.second <<'\n';
        std::cerr<< dest->position.first <<' '<< dest->position.second <<'\n';
        if (signX * (position.first - dest->position.first) <= 1 && signY * (position.second - dest->position.second) <= 1){
            IMHere(dest);

            ++currentInWay;
        }
    }
    else {
        travelling = false;
        onTheWay.clear();
        currentInWay = 0;
        bool temp = toTake;
        if (toDrop) {
            toDrop = false;
            drop(positionBuilding);
        }
        if (toTake) {
            if(positionBuilding->removeResource(needed)) {
                toTake = false;
                holding = needed;
                needed = Resource::DummyNothing;
            }
            else{
                currentTask = TaskID::Idle;
            }
        }
        switch (currentTask.id) {
            case TaskID::Transport:
                if(temp) {
                    moveToBuilding(currentTask.destination2);
                    toDrop = true;
                } else currentTask.id = TaskID::Idle;
                break;
            case TaskID::BeProcessed:
                //TODO:set pawn to be waiting, not free
                break;
            default:
                currentTask.id = TaskID::Idle;
        }
    }
}
void WorkerPawn::moveToBuilding(Building* dest) {
    travelling = true;
    std::unordered_map<Building*, std::vector<Building*> > visited;
    std::queue<Building*> q;
    visited[positionBuilding] = std::vector<Building*>();
    visited[positionBuilding].push_back(positionBuilding);
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
        if (currentB->parent== nullptr || !visited[currentB->parent].empty())
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
