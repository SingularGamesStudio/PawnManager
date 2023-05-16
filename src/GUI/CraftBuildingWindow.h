//
// Created by goodm on 30.03.2023.
//

#ifndef PAWNMANAGER_CRAFTBUILDINGWINDOW_H
#define PAWNMANAGER_CRAFTBUILDINGWINDOW_H

#include "ArrowControl.h"
#include "GameWindow.h"
#include "SlotControl.h"

class Recipe;

class CraftBuildingWindow : public GameWindow {
public:
    int id;
    int selectedRecipe;
    bool shouldClose;
    std::vector<SlotControl*> inputSlots;
    std::vector<SlotControl*> outputSlots;
    ArrowControl* arrow;

    explicit CraftBuildingWindow(int id);

    void updateAndRender() override;
private:
    int getRecipeCount();
    Recipe* getRecipe();
    bool isRecipeCurrent();
};


#endif//PAWNMANAGER_CRAFTBUILDINGWINDOW_H
