#ifndef WORKERPAWN_H
#define WORKERPAWN_H
///misha does not believe in me  :(
#include "Pawn.h"
#include <set>
#include <unordered_map>
#include <queue>
#include <vector>
enum class expertisesID{DummySmeltery, DummyMetalworking, DummtTrainership};
class WorkerPawn: public Pawn{
public:
    
    void create(Building* placeOfCreation);

    void assignInnerTask(Task toAssign);
    std::set<expertisesID> expertises;
    void moveToBuilding(Building* dest);
    void moveToNeighbour(Building* dest);
    void takeResourceFromBuilding(Building* dest, Resource res);
    void moveResourceTo(Building* dest); 
    std::vector<Building*> onTheWay;
    size_t currentInWay;
    void tick() override;
};
#endif //WORKERPAWN_H