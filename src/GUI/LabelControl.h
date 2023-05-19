//
// Created by goodm on 17.05.2023.
//

#ifndef PAWNMANAGER_LABELCONTROL_H
#define PAWNMANAGER_LABELCONTROL_H


#include "Control.h"
class LabelControl : public Control {
public:
    std::string label;
    LabelControl(GameWindow& win, sf::IntRect rect, std::string label);
    void updateAndRender() override;
};


#endif//PAWNMANAGER_LABELCONTROL_H
