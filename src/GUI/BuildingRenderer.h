//
// Created by goodm on 18.03.2023.
//

#ifndef PAWNMANAGER_BUILDINGRENDERER_H
#define PAWNMANAGER_BUILDINGRENDERER_H

#include "SFML/Graphics/RenderWindow.hpp"

class Building;

class BuildingRenderer {
    sf::RenderWindow& window;
public:
    explicit BuildingRenderer(sf::RenderWindow& window);
    void drawBuilding(Building* b, sf::Vector2f pos);
    void drawEdge(Building* a, Building* b, sf::Vector2f center);
};


#endif //PAWNMANAGER_BUILDINGRENDERER_H
