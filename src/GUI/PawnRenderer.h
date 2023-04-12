//
// Created by goodm on 16.03.2023.
//

#ifndef PAWNMANAGER_PAWNRENDERER_H
#define PAWNMANAGER_PAWNRENDERER_H
#include <set>

#include "Rect.h"
#include "RenderTarget.h"
#include "Vector.h"

enum class expertisesID;

class PawnRenderer {
private:
    RenderTarget& window;

public:
    explicit PawnRenderer(RenderTarget& window);
    void drawWorkerPawn(const std::set<expertisesID>& expertises, Vec2f pos);
};


#endif//PAWNMANAGER_PAWNRENDERER_H
