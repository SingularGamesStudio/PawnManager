//
// Created by goodm on 14.04.2023.
//

#ifndef PAWNMANAGER_BUILDINGRESULTCONTROL_H
#define PAWNMANAGER_BUILDINGRESULTCONTROL_H


#include "../Entities/Buildings/BuildingRegisty.h"
#include "Control.h"


class BuildingResultControl : public Control {
public:
    BuildingIdea* idea;
    BuildingResultControl(GameWindow &win, const sf::IntRect &rect);
    void updateAndRender() override;
};


#endif//PAWNMANAGER_BUILDINGRESULTCONTROL_H
