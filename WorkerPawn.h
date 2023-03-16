#include "Pawn.h"
#include <set>
#include <unordered_map>
#include <queue>
#include <vector>
enum class expertisesID{DummySmeltery, DummyMetalworking, DummtTrainership};
class WorkerPawn: public Pawn{
public:
    Building* positionBuilding;
    
    void create(Building* placeOfCreation);

    void assignInnerTask(Task toAssign);
    std::set<expertisesID> expertises;
    void moveToBuilding(Building* dest);
    void moveToNeighbour(Building* dest);
    void takeResourceFromBuilding(Building* dest, Resource res);
    void moveResourceTo(Building* dest);
};
