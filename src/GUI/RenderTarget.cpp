//
// Created by goodm on 12.04.2023.
//

#include "RenderTarget.h"

RenderTarget::RenderTarget(sf::RenderWindow& win) : win(win) {

}

void RenderTarget::draw(VertexArray arr) {
    arr.draw(win);
}
