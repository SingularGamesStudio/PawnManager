#include "ServerMain.h"

#include <unistd.h>

#include <chrono>
#include <unordered_map>

#include "Player.h"

using std::string;

int fromString(const string& str) {
    int result = 0;
    for (char c: str) { result = 10 * result + c - '0'; }
    return result;
}

bool GameServer::onConnection(std::shared_ptr<dlib::Connection> client) {}

void GameServer::onDisconnection(std::shared_ptr<dlib::Connection> client) {}

void GameServer::onPacketReceive(std::shared_ptr<dlib::Connection> client, dlib::Packet p) {}

int main(int argc, char** argv) {
    std::unordered_map<string, int> params;
    params["port"] = 57179;
    params["env"] = false;
    std::string prev = "";
    for (int i = 1; i < argc; ++i) {
        if (!prev.empty()) {
            params[prev] = fromString(string(argv[i]));
            prev = "";
        } else
            prev = string(argv[i]).substr(2);
    }
    GameServer server(params["port"]);
    server.start();
    auto currTime = std::chrono::steady_clock::now();
    while (1) {
        auto dTime = std::chrono::steady_clock::now() - currTime;
        currTime += dTime;
        /// TODO: Logic
        server.respond();
    }
    server.stop();
}