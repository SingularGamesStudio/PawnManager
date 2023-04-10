//
// Created by goodm on 10.04.2023.
//

#ifndef PAWNMANAGER_COLOR_H
#define PAWNMANAGER_COLOR_H
#include "SFML/Graphics/Color.hpp"
struct Color {
    int r, g, b, a;
    inline Color(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {}
    inline Color(int r, int g, int b) : Color(r, g, b, 255) {}
    inline explicit Color(int x) : Color(x, x, x) {}
    inline Color() : Color(0) {}
    inline explicit operator sf::Color() const { return sf::Color(r, g, b, a); }
    static const Color Black;
    static const Color Red;
    static const Color White;
    static const Color Green;
};
#endif//PAWNMANAGER_COLOR_H
