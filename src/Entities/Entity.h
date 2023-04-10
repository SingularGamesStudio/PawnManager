#ifndef ENTITY_H
#define ENTITY_H
#include <utility>

class Player;

class Entity {
private:
    virtual void destroy();

public:
    Entity() {}
    Entity(std::pair<double, double> pos, Player* owner, unsigned int hp, double radius) : position(pos), owner(owner), hp(hp), radius(radius) {}

    unsigned int hp;
    Player* owner;
    void changeHealth(int delta);

    std::pair<double, double> position;
    double radius;
};
#endif//ENTITY_H
