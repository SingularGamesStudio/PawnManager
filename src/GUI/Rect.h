//
// Created by goodm on 10.04.2023.
//

#ifndef PAWNMANAGER_RECT_H
#define PAWNMANAGER_RECT_H

#include "Vector.h"

template<typename T>
struct Rect2 {
    Vector2<T> pos;
    Vector2<T> size;
    Rect2(Vector2<T> pos, Vector2<T> size) : pos(pos), size(size) {}
    Rect2(Vector2<T> pos, T size) : Rect2(pos, Vector2<T>(size)) {}
    Rect2() : Rect2(Vector2<T>(0, 0), 0) {}
};

using Rect2f = Rect2<float>;
using Rect2i = Rect2<int>;

#endif//PAWNMANAGER_RECT_H
