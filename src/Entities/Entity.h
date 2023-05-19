#ifndef ENTITY_H
#define ENTITY_H
/// misha does not believe in me  :(
#include <cstring>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

#include "../Core/IDmanager.h"

struct Position {
    double x;
    double y;
    Position(double x, double y) : x(x), y(y) {}
    Position() : Position(0, 0) {}
    Position& operator+=(const Position& that) {
        x += that.x;
        y += that.y;
        return *this;
    }
    Position& operator-=(const Position& that) {
        x -= that.x;
        y -= that.y;
        return *this;
    }
    Position& operator*=(double k) {
        x *= k;
        y *= k;
        return *this;
    }
    Position& operator/=(double k) {
        x /= k;
        y /= k;
        return *this;
    }
    Position operator-() const {
        return {-x, -y};
    }

    Position operator+(const Position& that) const {
        Position result = *this;
        result += that;
        return result; // not returning result += because of RBO
    }
    Position operator-(const Position& that) const {
        Position result = *this;
        result -= that;
        return result; // not returning result -= because of RBO
    }
    Position operator*(double k) const {
        Position result = *this;
        result *= k;
        return result; // not returning result *= because of RBO
    }
    Position operator/(double k) const {
        Position result = *this;
        result /= k;
        return result; // not returning result /= because of RBO
    }
};


template<typename T>
size_t copyVariable(uint8_t* dst, T src) {
    std::memcpy(dst, &src, sizeof(src));
    return sizeof(src);
}
double dist(Position a, Position b);
template<typename T>
size_t initializeVariable(const uint8_t* src, T& dst) {
    std::memcpy(&dst, src, sizeof(dst));
    return sizeof(dst);
}


template<typename T>
size_t copyVector(uint8_t* dst, const std::vector<T>& src) {
    dst += copyVariable(dst, src.size());
    for (const T& i: src) { dst += copyVariable(dst, i); }
    return sizeof(size_t) + sizeof(T) * src.size();
}

template<typename T>
size_t initializeVector(const uint8_t* src, std::vector<T>& dst) {
    size_t size;
    src += initializeVariable(src, size);
    dst.resize(size);
    for (T& i: dst) { src += initializeVariable(src, i); }
    return sizeof(size_t) + sizeof(T) * size;
}


template<typename T>
size_t copySet(uint8_t* dst, const std::set<T>& src) {
    dst += copyVariable(dst, src.size());
    for (const T& i: src) { dst += copyVariable(dst, i); }
    return sizeof(size_t) + sizeof(T) * src.size();
}

template<typename T>
size_t initializeSet(const uint8_t* src, std::set<T>& dst) {
    size_t size;
    src += initializeVariable(src, size);
    for (size_t i = 0; i < size; ++i) {
        T tmp;
        src += initializeVariable(src, tmp);
        dst.insert(tmp);
    }
    return sizeof(size_t) + sizeof(T) * size;
}


template<typename T>
size_t copySet(uint8_t* dst, const std::multiset<T>& src) {
    dst += copyVariable(dst, src.size());
    for (const T& i: src) { dst += copyVariable(dst, i); }
    return sizeof(size_t) + sizeof(T) * src.size();
}

template<typename T>
size_t initializeSet(const uint8_t* src, std::multiset<T>& dst) {
    size_t size;
    src += initializeVariable(src, size);
    for (size_t i = 0; i < size; ++i) {
        T tmp;
        src += initializeVariable(src, tmp);
        dst.insert(tmp);
    }
    return sizeof(size_t) + sizeof(T) * size;
}

class Player;

class Entity : public RequiresID {
public:
    Entity() : position(Position(0, 0)), owner(ptr<Player>()), hp(30), radius(8) {}
    Entity(Position pos, ptr<Player> owner, double hp = 30, double radius = 8) : position(pos), owner(owner), hp(hp), radius(radius) {}
    double hp;
    ptr<Player> owner;
    Position position;
#ifdef CLIENT_SIDE
    ptr<Entity> motionTarget;
    double motionSpeed;
    double lastTime;
    void startMoveToEntity(ptr<Entity> e, double speed);
    Position getInterpolatedPos();
#endif
    double radius;
#ifdef SERVER_SIDE
    bool changeHealth(double delta);
#endif
    virtual ~Entity() = default;
    std::vector<uint8_t> serialize() const override;
    size_t deserialize(const uint8_t* data) override;

protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const uint8_t* data);
};
#endif// ENTITY_H
