//
// Created by goodm on 18.03.2023.
//

#ifndef PAWNMANAGER_RESOURCERENDERER_H
#define PAWNMANAGER_RESOURCERENDERER_H
#include "../Core/Resource.h"
#include "Rect.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "Vector.h"


class ResourceRenderer {
private:
    sf::RenderWindow& window;

public:
    explicit ResourceRenderer(sf::RenderWindow& window);
    void drawResource(Resource r, Vec2f pos, float rotation);
};


#endif//PAWNMANAGER_RESOURCERENDERER_H
