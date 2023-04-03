//
// Created by goodm on 18.03.2023.
//

#ifndef PAWNMANAGER_BUILDINGRENDERER_H
#define PAWNMANAGER_BUILDINGRENDERER_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "../IDmanager.h"

class Building;

class BuildingRenderer {
    sf::RenderWindow& window;
public:
    explicit BuildingRenderer(sf::RenderWindow& window);
    void drawBuilding(ptr<Building> b, sf::Vector2f pos);
    void drawEdge(ptr<Building> a, ptr<Building> b, sf::Vector2f center);
};


#endif //PAWNMANAGER_BUILDINGRENDERER_H
