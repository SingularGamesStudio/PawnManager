#ifndef ENTITY_H
#define ENTITY_H
/// misha does not believe in me  :(
#include <cstring>
#include <iostream>
#include <utility>
#include <vector>
#include <set>

#include "../IDmanager.h"

template<typename T>
size_t copyVariable(uint8_t* dst, T src) {
    std::memcpy(dst, &src, sizeof(src));
    return sizeof(src);
}

template<typename T>
size_t initializeVariable(const uint8_t* src, T& dst) {
    std::memcpy(&dst, src, sizeof(dst));
    return sizeof(dst);
}


template<typename T>
size_t copyVector(uint8_t* dst, const std::vector<T>& src) {
    dst += copyVariable(dst, src.size());
    for(const T& i : src) {
        dst += copyVariable(dst, i);
    }
    return sizeof(size_t) + sizeof(T) * src.size();
}

template<typename T>
size_t initializeVector(const uint8_t* src, std::vector<T>& dst) {
    size_t size;
    src += initializeVariable(src, size);
    dst.resize(size);
    for(T& i : dst) {
        src += initializeVariable(src, i);
    }
    return sizeof(size_t) + sizeof(T) * size;
}


template<typename T>
size_t copySet(uint8_t* dst, const std::set<T>& src) {
    dst += copyVariable(dst, src.size());
    for(const T& i : src) {
        dst += copyVariable(dst, i);
    }
    return sizeof(size_t) + sizeof(T) * src.size();
}

template<typename T>
size_t initializeSet(const uint8_t* src, std::set<T>& dst) {
    size_t size;
    src += initializeVariable(src, size);
    for(size_t i = 0; i < size; ++i) {
        T tmp;
        src += initializeVariable(src, tmp);
        dst.insert(tmp);
    }
    return sizeof(size_t) + sizeof(T) * size;
}


template<typename T>
size_t copySet(uint8_t* dst, const std::multiset<T>& src) {
    dst += copyVariable(dst, src.size());
    for(const T& i : src) {
        dst += copyVariable(dst, i);
    }
    return sizeof(size_t) + sizeof(T) * src.size();
}

template<typename T>
size_t initializeSet(const uint8_t* src, std::multiset<T>& dst) {
    size_t size;
    src += initializeVariable(src, size);
    for(size_t i = 0; i < size; ++i) {
        T tmp;
        src += initializeVariable(src, tmp);
        dst.insert(tmp);
    }
    return sizeof(size_t) + sizeof(T) * size;
}

class Player;

class Entity : public RequiresID {
public:
    Entity() {}
    Entity(std::pair<double, double> pos, ptr<Player> owner, double hp, double radius) : position(pos), owner(owner), hp(hp), radius(radius) {}
    double hp;
    ptr<Player> owner;
    std::pair<double, double> position;
#ifdef CLIENT_SIDE
    std::pair<double, double> prevPos;
    double posReachTime;
    double beginTime;
    void startMoveToPos(std::pair<double, double> pos, double time);
    std::pair<double, double> getInterpolatedPos();
#endif
    double radius;
#ifdef SERVER_SIDE
    void changeHealth(double delta);
#endif
    virtual ~Entity() = default;
    std::vector<uint8_t> serialize() const override;
    size_t deserialize(const uint8_t* data) override;

protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const uint8_t* data);
};
#endif// ENTITY_H
