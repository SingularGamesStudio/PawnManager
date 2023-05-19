//
// Created by goodm on 13.04.2023.
//

#ifndef PAWNMANAGER_BUILDBUILDINGWINDOW_H
#define PAWNMANAGER_BUILDBUILDINGWINDOW_H

#include "ArrowControl.h"
#include "BuildingResultControl.h"
#include "GameWindow.h"
#include "SlotControl.h"
class BuildBuildingWindow : public GameWindow {
    int id;
    sf::Vector2f pos;
    int selectedRecipe;
    bool shouldClose;
    std::vector<SlotControl*> inputSlots;
    BuildingResultControl* result;
    ArrowControl* arrow;

public:
    BuildBuildingWindow(int id, const sf::Vector2f& pos);
    void updateAndRender() override;
};


#endif//PAWNMANAGER_BUILDBUILDINGWINDOW_H
