#include "Player.h"
#include <utility>

class Entity {
private:
    virtual void destroy(){}

public:
    unsigned int hp;
    Player owner;
    void changeHealth(int delta){}

    std::pair<double, double> position;
    double radius;
};
