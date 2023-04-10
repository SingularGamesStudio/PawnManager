//
// Created by goodm on 10.04.2023.
//

#ifndef PAWNMANAGER_VERTEXARRAY_H
#define PAWNMANAGER_VERTEXARRAY_H

#include "SFML/Graphics/VertexArray.hpp"
#include "Vector.h"
#include "Color.h"
#include "SFML/Graphics/RenderWindow.hpp"

class VertexArray {
    sf::VertexArray arr;
public:
    VertexArray();
    void appendVertex(Vec2f pos, Color col);
    void draw(sf::RenderWindow& win);
};


#endif //PAWNMANAGER_VERTEXARRAY_H
