#ifndef WORKERPAWN_H
#define WORKERPAWN_H

#include "Pawn.h"

#include <cstddef>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

enum class expertisesID{DummySmeltery, DummyMetalworking, DummtTrainership};

class WorkerPawn: public Pawn{
public:

    std::set<expertisesID> expertises;
    bool toDrop;
    bool toTake;
    std::vector<Building*> onTheWay;
    size_t currentInWay;

    void create(Building* placeOfCreation);

    void assignTask(const Task& toAssign) override;

    void moveToBuilding(Building* dest) override;

    void tick(double deltaTime) override;
};

#endif //WORKERPAWN_H