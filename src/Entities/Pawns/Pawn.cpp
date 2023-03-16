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
    if (inside != nullptr && holding != Resource::DummyNothing) {
        drop(inside, position);
    }
    else if (holding != Resource::DummyNothing) {
        drop(inside, position);
    }
    ///TODO task not done... god object taskmanager not happy :(
    //~Pawn();
    }
void Pawn::IMNotHere(Building* from) {
    if (from != nullptr)
        from->removePawn(this);
}
void Pawn::IMHere(Building* to) {
    if (to != nullptr)
        to->addPawn(this);
}
void Pawn::beIngridient() {
    assignTask(Task(TaskID::BeProcessed, inside));
}
void Pawn::stopBeingIngridient() {
    assignTask(Task(TaskID::Idle,inside));
}
void Pawn::moveToBuilding(Building* toMove) {
    throw("How did we get here?");
}
void Pawn::assignTask(const Task& toAssign) {}