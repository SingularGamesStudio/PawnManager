//
// Created by goodm on 13.04.2023.
//

#ifndef PAWNMANAGER_SLOTCONTROL_H
#define PAWNMANAGER_SLOTCONTROL_H
#include "Control.h"
#include "../Resource.h"

class SlotControl : public Control {
public:
    Resource res;
    SlotControl(GameWindow& win, const sf::IntRect& rect);

    void updateAndRender() override;
};


#endif //PAWNMANAGER_SLOTCONTROL_H
