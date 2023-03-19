#include "Pawn.h"
#include "../../Resource.h"
#include "../ResourceEntity.h"
#include "../Buildings/Building.h"


void Pawn::drop(Building* in, std::pair<double, double> pos) {
    if (in != nullptr && holding != Resource::DummyNothing) {
        in->addResource(holding);
        holding = Resource::DummyNothing;
        return;
    }
    if (holding != Resource::DummyNothing)
        ResourceEntity(holding, pos);
}
void Pawn::destroy() {
    if (positionBuilding != nullptr && holding != Resource::DummyNothing) {
        drop(positionBuilding, position);
    }
    else if (holding != Resource::DummyNothing) {
        drop(positionBuilding, position);
    }
    ///TODO task not done... god object taskmanager not happy :(
    //~Pawn();
    }
void Pawn::IMNotHere() {
    if (positionBuilding != nullptr) {
        positionBuilding->removePawn(this);
        positionBuilding = nullptr;
    }
}
void Pawn::IMHere(Building* to) {
    if (to != nullptr) {
        to->addPawn(this);
        IMNotHere();
        positionBuilding = to;
        position = positionBuilding->position;
    }
}
void Pawn::beIngridient() {
    assignTask(Task(TaskID::BeProcessed, positionBuilding));
}
void Pawn::stopBeingIngridient() {
    assignTask(Task(TaskID::Idle,positionBuilding));
}
void Pawn::moveToBuilding(Building* toMove) {
    throw("How did we get here?");
}
//void Pawn::assignTask(const Task& toAssign) = 0;

void Pawn::tick(double deltaTime) {

}
