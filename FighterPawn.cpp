#include "FighterPawn.h"
void DummyMonk::attack(Entity* attacked) override;
FighterPawnType DummyMonk::getType() override {
    return FighterPawnType::DummyMonk;
}
void DummySwordsman::attack(Entity* attacked) override;
FighterPawnType DummySwordsman::getType() override {
    return FighterPawnType::DummySwordsmans;
}
FighterPawn* FighterPawn::createFighterPawn(FighterPawnType type, Building* placeOfCreation) {
    switch (id) {
    case DummyMonk:
        this = new DummyMonk();
    case DummySwordsman:
        this = new DummySwordsman();
    default:
        throw("Type of FighterPawn not found");
    }
    currentTask = Task(TaskID::Idle, placeOfCreation);
    travelling = false;
    holding = nullptr;
    owner = placeOfCreation->owner;
    destination = placeOfCreation;
    inside = placeOfCreation;
}
void FighterPawn::getResource(ResourceEntity* toGet) {
    if (inside != nullptr)
        ImNotHere(inside);
    moveToResource(toGet);
    takePresentResource(toGet);
    moveToBuilding(owner->hub);
    inside = hub;
    hub->addPawn(this);
    drop();
}
void FighterPawn::moveToResource(ResourceEntity* toGet) {
    moveToPosition(toGet->position);
}
void FighterPawn::takePresentResource(ResourceEntity* toTake) {
    holding = toTake->resource;
    toTake->destroy();
}
void FighterPawn::moveToPosition(std::pair<double, double> pos) {
    IMNotHere(inside);
    double currentTime = 0;
    double timePerMove = sqrt(abs(position.first - pos.first) * abs(position.first - pos.first) +
        abs(position.second - pos.second) * abs(position.second - pos.second));
    while (currentTime < timePerMove) {
        currentTime += timePerMove / ticksPerSecond;
        position.first = currentTime / ticksPerSecond * (*positionBuilding).position.first
            + (1 - currentTime / ticksPerSecond) * (*positionBuilding).position.first;
        position.second = currentTime / ticksPerSecond * (*dest).position.second
            + (1 - currentTime / ticksPerSecond) * (*dest).position.second;
    }
    position = pos;

}
void FighterPawn::moveToBuilding(Building* dest) override {
    moveToPosition(dest->position);
    IMHere(dest);
}
