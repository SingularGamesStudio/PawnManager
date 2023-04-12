//
// Created by goodm on 12.04.2023.
//

#ifndef PAWNMANAGER_RENDERTARGET_H
#define PAWNMANAGER_RENDERTARGET_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "VertexArray.h"

class RenderTarget {
private:
    sf::RenderWindow& win;
public:
    explicit RenderTarget(sf::RenderWindow& win);
    void draw(VertexArray arr);
};


#endif //PAWNMANAGER_RENDERTARGET_H
