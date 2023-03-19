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
    std::cout << "task " << (int)toAssign.id << "\n";
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
        position.first+= (dest->position.first - positionBuilding->position.first) * speed * deltaTime;
        position.second+= (dest->position.second - positionBuilding->position.second) * speed * deltaTime;

        if (fabs(position.first - dest->position.first) < 1e-1 && fabs(position.second - dest->position.second) < 1e-1){
            IMHere(dest);

            ++currentInWay;
        }
    }
    else {
        travelling = false;
        onTheWay.clear();
        currentInWay = 0;
        if (toDrop) {
            toDrop = false;
            drop(positionBuilding);
        }
        if (toTake) {
            toTake = false;
            holding = needed;
            positionBuilding->removeResource(holding);
            needed = Resource::DummyNothing;
        }
        switch (currentTask.id) {
            case TaskID::Transport:
                moveToBuilding(currentTask.destination2);
                toDrop = true;
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
