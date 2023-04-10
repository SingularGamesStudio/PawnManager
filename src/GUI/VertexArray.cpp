//
// Created by goodm on 10.04.2023.
//

#include "VertexArray.h"

VertexArray::VertexArray() : arr() { arr.setPrimitiveType(sf::PrimitiveType::Triangles); }

void VertexArray::appendVertex(Vec2f pos, Color col) { arr.append(sf::Vertex(static_cast<sf::Vector2f>(pos), static_cast<sf::Color>(col))); }

void VertexArray::draw(sf::RenderWindow& win) { win.draw(arr); }
