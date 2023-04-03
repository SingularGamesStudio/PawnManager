#include "Pawn.h"
#include "../../Resource.h"
#include "../ResourceEntity.h"
#include "../Buildings/Building.h"


void Pawn::drop(ptr<Building> in, std::pair<double, double> pos) {
    if (in && holding != Resource::DummyNothing) {
        in->addResource(holding);
        holding = Resource::DummyNothing;
        return;
    }
    if (holding != Resource::DummyNothing)
        ResourceEntity(holding, pos);
}
void Pawn::destroy() {
    if (positionBuilding && holding != Resource::DummyNothing) {
        drop(positionBuilding, position);
    }
    else if (holding != Resource::DummyNothing) {
        drop(positionBuilding, position);
    }
    ///TODO task not done... god object taskmanager not happy :(
    //~Pawn();
    }
void Pawn::IMNotHere() {
    if (positionBuilding) {
        positionBuilding->removePawn(ptr<Pawn>(id));
        positionBuilding = ptr<Building>();
    }
}
void Pawn::IMHere(ptr<Building> to) {
    if (to) {
        to->addPawn(ptr<Pawn>(id));
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
void Pawn::moveToBuilding(ptr<Building> toMove) {
    throw("How did we get here?");
}
//void Pawn::assignTask(const Task& toAssign) = 0;

void Pawn::tick(double deltaTime) {

}

void Pawn::deserialize(const std::vector<uint8_t>& data) const {

}

std::vector<uint8_t> Pawn::serialize() const {

}