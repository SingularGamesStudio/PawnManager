#ifndef ENTITY_H
#define ENTITY_H
///misha does not believe in me  :(
#include <utility>
#include "../IDmanager.h"
#include <iostream>

class Player;

class Entity: public RequiresID{
private:
    virtual void destroy();

public:
    Entity(){
    }
    Entity(std::pair<double, double> pos, ptr<Player> owner, unsigned int hp, double radius):position(pos), owner(owner), hp(hp), radius(radius){
    }
    int hp;
    ptr<Player> owner;
    void changeHealth(int delta);

    std::pair<double, double> position;
    double radius;
    virtual std::vector<uint8_t> serialize() const;
    virtual void deserialize(const std::vector<uint8_t>& data);
    virtual ~Entity();
};
#endif //ENTITY_H
