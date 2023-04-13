//
// Created by goodm on 13.04.2023.
//

#ifndef PAWNMANAGER_ARROWCONTROL_H
#define PAWNMANAGER_ARROWCONTROL_H
#include <set>

#include "../Entities/Pawns/WorkerPawn.h"
#include "Control.h"

class ArrowControl : public Control {
public:
    ArrowControl(GameWindow& win, const sf::IntRect& rect);
    std::vector<expertisesID> pawnExpertises;

private:
    void updateAndRender() override;
};


#endif//PAWNMANAGER_ARROWCONTROL_H
