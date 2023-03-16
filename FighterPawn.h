#include "Building.h"
#include "Pawn.h"
#include <cmath>
enum class FighterPawnType{
    DummyMonk,
    DummySwordsman
};
class FighterPawn : public Pawn{
public:
    virtual FighterPawn* createFighterPawn(FighterPawnType type);
    double atk;
    double speed;
    void virtual attack(Entity* attacked);
    virtual FighterPawnType getType();
    static FighterPawn* createFighterPawn(FighterPawnType type, Building* placeOfCreation);
    void moveToResource(ResourceEntity* toGet);
    void takePresentResource(ResourceEntity* toTake);
    void moveToPosition(std::pair<double, double> pos);
    void moveToBuilding(Building* dest) override;
}
class DummyMonk : public FighterPawn {
public:
    void attack(Entity* attacked) override;
    FighterPawnType getType() override;
};
class DummySwordsman : public FighterPawn {
public:
    void attack(Entity* attacked) override;
    FighterPawnType getType() override;
};