//
// Created by goodm on 30.03.2023.
//

#ifndef PAWNMANAGER_CRAFTBUILDINGWINDOW_H
#define PAWNMANAGER_CRAFTBUILDINGWINDOW_H

#include "GameWindow.h"

class CraftBuildingWindow : public GameWindow {
    int id;

public:
    explicit CraftBuildingWindow(int id);

    void updateAndRender() override;
};


#endif //PAWNMANAGER_CRAFTBUILDINGWINDOW_H
