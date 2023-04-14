#ifndef PAWNMANAGER_LOCALCONTROLLER_H
#define PAWNMANAGER_LOCALCONTROLLER_H
#include <set>

#include "Entities/ResourceEntity.h"
#include "Event.h"
#include "Player.h"
#include "net/client.h"

class LocalController : dlib::ClientInterface {
public:
    std::set<ptr<Player>> players;
    std::set<ptr<ResourceEntity>> danglingResources;
    ptr<Player> mainPlayer;

    void onPacketReceive(const dlib::Packet &p) override;
};

#endif//PAWNMANAGER_LOCALCONTROLLER_H
