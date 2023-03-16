#include "Building.h"
#include "Pawn.h"
enum class FighterPawnType{
    DummyMonk,
    DummySwordsman};
class FighterPawn : public Pawn{
public:
    virtual FighterPawn* createFighterPawn(FighterPawnType type);
    double atk;
    double speed;
    void virtual attack(Entity* attacked);
}
class DummyMonk : public FighterPawn{
public:
    void attack(Entity* attacked) override;
};
class DummySwordsman : public FighterPawn{
public:
    void attack(Entity* attacked) override;
};
static FighterPawn* FighterPawn::createFighterPawn(FighterPawnType type){
    switch(id){
        case DummyMonk:
            return (new DummyMonk());
        case DummySwordsman:
            return (new DummySwordsman());
        default:
            throw("Type of Fighter pawn not Found");
    }
    void getResource(ResourceEntity* toGet) {
        if (inside != nullptr)
            ImNotHere(inside);
        moveToResource(toGet);
        takePresentResource(toGet);
        //пойти в хаб
    }
    void moveToResource(Resource* toGet) {

    }
    void takePresentResource(ResourceEntity* toTake) {
        holding = toTake->resource;
        //убрать ресурс≈нтитю с карты
    }
    void moveToPosition(std::pair<double, double> pos) {

    }
}
