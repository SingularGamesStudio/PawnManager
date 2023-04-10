//
// Created by goodm on 10.04.2023.
//

#ifndef PAWNMANAGER_VECTOR_H
#define PAWNMANAGER_VECTOR_H

#include "SFML/System/Vector2.hpp"
#include "cmath"

template <typename T>
struct Vector2 {
    T x, y;
    Vector2(T x, T y) : x(x), y(y) {}
    explicit Vector2(T a) : Vector2(a, a) {}
    Vector2() : Vector2(0) {}
    explicit Vector2(sf::Vector2<T> a) : Vector2(a.x, a.y) {}
    template<typename U>
    explicit Vector2(Vector2<U> v) : x(v.x), y(v.y) {}
    Vector2<T>& operator+=(Vector2<T> b) {
        x += b.x;
        y += b.y;
        return *this;
    }
    Vector2<T> operator-() const {
        return {-x, -y};
    }
    Vector2<T>& operator-=(Vector2<T> b) {
        x -= b.x;
        y -= b.y;
        return *this;
    }
    Vector2<T>& operator*=(T a) {
        x *= a;
        y *= a;
        return *this;
    }
    Vector2<T>& operator/=(T a) {
        x /= a;
        y /= a;
        return *this;
    }
    explicit operator sf::Vector2<T>() const {
        return {x, y};
    }
};

template <typename T>
Vector2<T> operator+(Vector2<T> a, Vector2<T> b) {
    return a += b;
}

template <typename T>
Vector2<T> operator-(Vector2<T> a, Vector2<T> b) {
    return a -= b;
}

template <typename T>
Vector2<T> operator*(Vector2<T> a, T b) {
    return a *= b;
}

template <typename T>
Vector2<T> operator*(T a, Vector2<T> b) {
    return b *= a;
}

template<typename T>
Vector2<T> operator/(Vector2<T> a, T b) {
    return a /= b;
}

template <typename T>
T dot(Vector2<T> a, Vector2<T> b) {
    return a.x * b.x + a.y * b.y;
}

template <typename T>
T sqrAbs(Vector2<T> a) {
    return dot(a, a);
}

template <typename T>
T abs(Vector2<T> a) {
    return std::sqrt(sqrAbs(a));
}

using Vec2f = Vector2<float>;
using Vec2i = Vector2<int>;

#endif //PAWNMANAGER_VECTOR_H
