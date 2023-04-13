#ifndef ENTITY_H
#define ENTITY_H
/// misha does not believe in me  :(
#include <iostream>
#include <utility>
#include <vector>

#include "../IDmanager.h"

template<typename T>
size_t copyVariable(uint8_t* dst, T src);

template<typename T>
size_t initializeVariable(const uint8_t* src, T& dst);

class Player;

class Entity : public RequiresID {
public:
    Entity() {}
    Entity(std::pair<double, double> pos, ptr<Player> owner, double hp, double radius) : position(pos), owner(owner), hp(hp), radius(radius) {}
    double hp;
    ptr<Player> owner;
    std::pair<double, double> position;
    double radius;

    void changeHealth(int delta);
    virtual ~Entity();
    virtual std::vector<uint8_t> serialize() const;
    virtual size_t deserialize(const std::vector<uint8_t> &data);
protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const std::vector<uint8_t> &data);
};
#endif// ENTITY_H
