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
    destination = placeOfCreation;
    IMHere(placeOfCreation);
}
void WorkerPawn::assignTask(const Task& toAssign) {
    std::cout << "got task " << (int)toAssign.id << "\n";
    currentTask = toAssign;
    switch (toAssign.id) {
    case TaskID::Get:
        moveToBuilding(toAssign.destination);
        needed = toAssign.object;
        toTake = true;
        break;
    case TaskID::Transport:
        moveToBuilding(toAssign.destination);
        moveToBuilding(toAssign.destination2);
        toDrop = true;
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
        position.second+= (dest->position.first - positionBuilding->position.first) * speed * deltaTime;
        if (fabs(position.first - dest->position.first) < 1e-6 && fabs(position.second - dest->position.second) < 1e-6){
            IMHere(dest);
            ++currentInWay;//TODO:fix from here
            /*if (currentInWay < onTheWay.size() && onTheWay[currentInWay] == destination) {
                takeResourceFromBuilding(needed);

            }*/
            if(currentInWay < onTheWay.size())
                IMNotHere();
        }
    }
    else {
        travelling = false;
        if (toDrop) {
            toDrop = false;
            drop(positionBuilding);
        }
        if (toTake) {
            holding = needed;
            positionBuilding->removeResource(holding);
            needed = Resource::DummyNothing;
        }
        onTheWay.clear();
        currentInWay = 0;
    }
}
void WorkerPawn::moveToBuilding(Building* dest) {
    travelling = true;
    std::unordered_map<Building*, std::vector<Building*> > visited;
    std::queue<Building*> q;
    if(onTheWay.empty())
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
    std::cout << "finished way search" << "\n";
}
