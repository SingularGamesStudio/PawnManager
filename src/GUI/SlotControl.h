//
// Created by goodm on 13.04.2023.
//

#ifndef PAWNMANAGER_SLOTCONTROL_H
#define PAWNMANAGER_SLOTCONTROL_H
#include "Control.h"
#include "../Resource.h"
#include <set>
#include "../Entities/Pawns/WorkerPawn.h"
#include "../Entities/Pawns/FighterPawn.h"

class SlotControl : public Control {
public:
    Resource res;
    std::set<expertisesID> pawnExpertiese;
    FighterPawnType fighterPawnType;
    int drawType;
    SlotControl(GameWindow& win, const sf::IntRect& rect);

    void updateAndRender() override;
};


#endif //PAWNMANAGER_SLOTCONTROL_H
