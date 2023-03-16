#include "Pawn.h"
#include "Task.h"
#include <vector>

class Player {
public:
    Building* hub;
    void addPawn(Pawn* pawn){
        //TODO
    }
    void removePawn(Pawn* pawn, bool busy){ //if busy, then it is working on something, otherwise, it is destroyed
        //TODO
    }
    void addResource(Resource resource, Building* where){
        //TODO
    }

    void removeResource(Resource* resource){
        //TODO
    }
    void assignTask(TaskID id) {
        //TODO
    }
};
