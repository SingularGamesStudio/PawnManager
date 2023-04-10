#include "Pawn.h"

#include "../../Resource.h"
#include "../Buildings/Building.h"
#include "../ResourceEntity.h"

void Pawn::drop(Building* in, std::pair<double, double> pos) {
    if (in != nullptr && holding != Resource::DummyNothing) {
        in->addResource(holding);
        holding = Resource::DummyNothing;
        return;
    }
    if (holding != Resource::DummyNothing) ResourceEntity(holding, pos);
}

void Pawn::destroy() {
    if (positionBuilding != nullptr && holding != Resource::DummyNothing) {
        drop(positionBuilding, position);
    } else if (holding != Resource::DummyNothing) {
        drop(positionBuilding, position);
    }

    ///TODO task not done... god object taskmanager not happy :(
}

void Pawn::GetOutOfBuilding() {
    if (positionBuilding != nullptr) {
        positionBuilding->removePawn(this);
        positionBuilding = nullptr;
    }
}

void Pawn::GetIntoBuilding(Building* to) {
    if (to != nullptr) {
        to->addPawn(this);
        GetOutOfBuilding();
        positionBuilding = to;
        position = positionBuilding->position;
    }
}

void Pawn::beIngridient() { assignTask(Task(TaskID::BeProcessed, positionBuilding)); }

void Pawn::stopBeingIngridient() { assignTask(Task(TaskID::Idle, positionBuilding)); }

void Pawn::moveToBuilding(Building* toMove) { throw("How did we get here?(moveToBuilding)"); }

void Pawn::tick(double deltaTime) { throw("How did we get here?(tick)"); }
