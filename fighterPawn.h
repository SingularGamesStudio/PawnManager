//
// Created by admin on 09.03.2023.
//

#ifndef PAWNMANAGER_FIGHTERPAWN_H
#define PAWNMANAGER_FIGHTERPAWN_H
enum class FighterPawnType{DummyMonk,DummySwordsman};
class FighterPawn(){
public:
    virtual FighterPawn* createFighterPawn(FighterPawnType type);
    double atk;
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
FighterPawn* FighterPawn::createFighterPawn(FighterPawnType type){
    switch(id){
        case DummyMonk:
            return (new DummyMonk());
        case DummySwordsman:
            return (new DummySwordsman());
        default:
            throw("Type of Fighter pawn not Found");
    }
}
#endif //PAWNMANAGER_FIGHTERPAWN_H
