//
// Created by goodm on 17.05.2023.
//

#ifndef PAWNMANAGER_FIGHTERPAWNCONTROLWINDOW_H
#define PAWNMANAGER_FIGHTERPAWNCONTROLWINDOW_H


#include "../Core/IDmanager.h"
#include "GameWindow.h"
#include <map>
#include "../Entities/Pawns/FighterPawn.h"
#include "SlotControl.h"
#include "LabelControl.h"
#include "TextInputControl.h"

class FighterPawnControlWindow : public GameWindow {
public:
    int currentSelected;
    ptr<Building> b;
    SlotControl* slot;
    LabelControl* pawnCnt;
    TextInputControl* countInput;
    bool shouldClose;
    explicit FighterPawnControlWindow(ptr<Building> b);
    void updateAndRender() override;
    bool isProtection();
    std::vector<std::pair<FighterPawnType, int>> getPawnCnts();
};


#endif//PAWNMANAGER_FIGHTERPAWNCONTROLWINDOW_H
