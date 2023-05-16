//
// Created by goodm on 18.03.2023.
//

#ifndef PAWNMANAGER_RESOURCERENDERER_H
#define PAWNMANAGER_RESOURCERENDERER_H
#include "../Core/Resource.h"
#include "SFML/Graphics/RenderWindow.hpp"


class ResourceRenderer {
private:
    sf::RenderWindow& window;

public:
    explicit ResourceRenderer(sf::RenderWindow& window);
    void drawResource(Resource r, sf::Vector2f pos, float rotation, float sz = 7);
};


#endif//PAWNMANAGER_RESOURCERENDERER_H
