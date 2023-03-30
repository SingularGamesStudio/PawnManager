#ifndef ENTITY_H
#define ENTITY_H
///misha does not believe in me  :(
#include <utility>
#include "../IDmanager.h"
#include <iostream>

class Player;

class Entity {
private:
    virtual void destroy();

public:
    Entity():id(IDmanager::newEntity(this)){
    }
    Entity(std::pair<double, double> pos, Player* owner, unsigned int hp, double radius):position(pos), owner(owner), hp(hp), radius(radius), id(IDmanager::newEntity(this)){
    }

    int id;
    unsigned int hp;
    Player* owner;
    void changeHealth(int delta);

    std::pair<double, double> position;
    double radius;
};
#endif //ENTITY_H
