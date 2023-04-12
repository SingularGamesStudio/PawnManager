//
// Created by goodm on 18.03.2023.
//

#ifndef PAWNMANAGER_BUILDINGRENDERER_H
#define PAWNMANAGER_BUILDINGRENDERER_H

#include "Rect.h"
#include "RenderTarget.h"
#include "Vector.h"

class Building;

class BuildingRenderer {
    RenderTarget& window;

public:
    explicit BuildingRenderer(RenderTarget& window);
    void drawBuilding(Building* b, Vec2f pos);
    void drawEdge(Building* a, Building* b, Vec2f center);
};


#endif//PAWNMANAGER_BUILDINGRENDERER_H
