#include <random>
#include <unordered_map>

#include "IDmanager.h"
#include "net/server.h"

class Player;

class GameServer : public dlib::ServerInterface {
public:
    std::unordered_map<int, ptr<Player>> players;
    std::mt19937 rnd;
    GameServer(uint16_t port) : dlib::ServerInterface(port) {}
    virtual bool onConnection(std::shared_ptr<dlib::Connection> client) override;
    virtual void onDisconnection(std::shared_ptr<dlib::Connection> client) override;
    virtual void onPacketReceive(std::shared_ptr<dlib::Connection> client, dlib::Packet p) override;

    void tick(double deltaTime);
};

int main(int argc, char** argv);// takes port and whether there should be environment monsters