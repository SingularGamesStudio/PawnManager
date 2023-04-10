#ifndef ENTITY_H
#define ENTITY_H
#include <utility>

class Player;

struct Position {
    double x;
    double y;
    Position(double x, double y) : x(x), y(y) {}
    Position() : Position(0, 0) {}
};

class Entity {
private:
    virtual void destroy();

public:
    Entity() {}
    Entity(Position pos, Player* owner, unsigned int hp, double radius) : position(pos), owner(owner), hp(hp), radius(radius) {}

    unsigned int hp;
    Player* owner;
    void changeHealth(int delta);

    Position position;
    double radius;
};
#endif//ENTITY_H
