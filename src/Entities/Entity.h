#ifndef ENTITY_H
#define ENTITY_H
/// misha does not believe in me  :(
#include <iostream>
#include <utility>
#include <vector>

#include "../IDmanager.h"

class Player;

class Entity : public RequiresID {
public:
    Entity() {}
    Entity(std::pair<double, double> pos, ptr<Player> owner, double hp, double radius) : position(pos), owner(owner), hp(hp), radius(radius) {}
    double hp;
    ptr<Player> owner;
    void changeHealth(double delta);

    std::pair<double, double> position;
    double radius;
    virtual std::vector<uint8_t> serialize() const;
    virtual void deserialize(const std::vector<uint8_t> &data);
    virtual ~Entity() = default;
};
#endif// ENTITY_H
