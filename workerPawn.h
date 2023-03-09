//
// Created by admin on 09.03.2023.
//

#ifndef PAWNMANAGER_WORKERPAWN_H
#define PAWNMANAGER_WORKERPAWN_H
#include <set>
enum class expertisesID{DummySmeltery, DummyMetalworking, DummtTrainership};
class WorkerPawn: public Entity{
public:
    Building* destination;
    Building* position;
    double howFarOnEdge;
    bool travelling;
    Resource* holding;
    Task CurrentTask;
    void assignTask(Task toAssign);
    std::set<expertisesID> expertises;
private:
    void getResource(Resource* toGet);
    void moveTo(Building* destination);
    void takePresentResource(Resource* toTake);

};
#endif //PAWNMANAGER_WORKERPAWN_H
