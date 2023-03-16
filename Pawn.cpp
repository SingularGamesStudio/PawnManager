Pawn::void drop(Building* in, std::pair<double, double> pos = { 0,0 }) {
    if (in != nullptr && holding != Resource::DummyNothing) {
        in->addResource(holding);
        holding = Resource::DummyNothing;
        return;
    }
    if (holding != Resource::DummyNothing)
        ResourceEntity(holding, pos);
}
Pawn::void destroy() {
    if (inside != nullptr && holding != Resorce::DummyNothing) {
        (*holding).drop(inside);
    }
    else if (holding != Resource::DummyNothing) {
        (*holding).drop(position);
    }
    ///TODO task not done... god object taskmanager not happy :(
    ~Pawn();
    }
Pawn::void IMNotHere(Building* from) {
    if (from != nullptr)
        from->removePawn(this);
}
Pawn::void IMHere(Building* to) {
    if (to != nullptr)
        to->addPawn(this);
}
Pawn::virtual void moveToBuilding(Building* toMove) = 0;
Pawn::virtual void assignTask(const Task& toAssign) = 0;
Pawn::void beIngridient() {
    assignTask(TaskID::BeProcessed);
}
Pawn::void stopBeingIngridient() {
    assignTask(TaskID::Idle);
}