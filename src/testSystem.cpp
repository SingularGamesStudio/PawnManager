#include "Player.h"
#include "Entities/Buildings/Building.h"


Player* initTest() {
    Player* player = new Player();
    player->hub = new Building({1000, 1000}, player, 100);
    for(int i = 0; i<30; i++){
        player->hub->addResource(Resource::DummyOre);
    }
}

void tick(double time) {

}