#include "net/server.h"

class GameServer : public dlib::ServerInterface {
public:
    GameServer(uint16_t port) : dlib::ServerInterface(port) {}
    virtual bool onConnection(std::shared_ptr<dlib::Connection> client) override;
    virtual void onDisconnection(std::shared_ptr<dlib::Connection> client) override;
    virtual void onPacketReceive(std::shared_ptr<dlib::Connection> client, dlib::Packet p) override;
};

int main(int argc, char** argv); // takes port and whether there should be environment monsters