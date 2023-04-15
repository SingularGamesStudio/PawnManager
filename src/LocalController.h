#ifndef PAWNMANAGER_LOCALCONTROLLER_H
#define PAWNMANAGER_LOCALCONTROLLER_H
#include <set>

#include "Entities/ResourceEntity.h"
#include "Event.h"
#include "Player.h"
#include "net/client.h"

class LocalController : public dlib::ClientInterface {
public:
    std::set<ptr<Player>> players;
    std::set<ptr<ResourceEntity>> danglingResources;
    ptr<Player> mainPlayer;

    void onPacketReceive(const dlib::Packet &p) override;

    void sendPacket(Recipe* recipe, ptr<Building> where);

    void init(std::string host, uint16_t port);
};

#endif//PAWNMANAGER_LOCALCONTROLLER_H
