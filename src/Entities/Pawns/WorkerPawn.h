#ifndef WORKERPAWN_H
#define WORKERPAWN_H
///misha does not believe in me  :(
#include "Pawn.h"
#include <set>
#include <unordered_map>
#include <queue>
#include <vector>
#include <cstddef>
enum class expertisesID{DummySmeltery, DummyMetalworking, DummyTrainership};
class WorkerPawn: public Pawn{
public:
    
    void create(Building* placeOfCreation);

    void assignTask(const Task& toAssign) override;
    std::set<expertisesID> expertises;
    void moveToBuilding(Building* dest) override;
    std::vector<Building*> onTheWay;
    size_t currentInWay;
    void tick(double deltaTime) override;
};
#endif //WORKERPAWN_H