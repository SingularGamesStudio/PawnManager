#pragma once
#include "general.h"
#include "packet.h"
#include "mqueue.h"
#include "connection.h"

namespace dlib {

    class serverInterface {
    protected:
        boost::asio::io_context serverContext;
        std::thread contextThread;
        boost::asio::ip::tcp::acceptor serverAcceptor;
        MQueue<std::pair<Packet, std::shared_ptr<Connection>>> inQueue;
        std::vector<std::shared_ptr<Connection>> connections;
    public:
        serverInterface(uint16_t port) : serverAcceptor(serverContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
        }
        virtual ~serverInterface() {
            stop();
        }

        bool start() {
            try {
                waitForConnections();

                contextThread = std::thread([this]() { serverContext.run(); });
            }
            catch(std::exception& e) {
                std::cerr << "SERVER threw an exception whilst trying to start:\n\t" << e.what() << "\n";
                return false;
            }
            std::cout << "SERVER started successfully!\n";
            return true;
        }

        void stop() {
            serverContext.stop();
            if(contextThread.joinable())
                contextThread.join();
            std::cout << "SERVER stopped...\n";
        }

        void sendPacketClient(std::shared_ptr<Connection> client, const Packet& p) {
            if (client && client->isConnected()){
                client->send(p);
                return;
            }
            onDisconnection(client);
            client.reset();
            connections.erase(std::remove(connections.begin(), connections.end(), client), connections.end());
        }

        void sendPacketAll(const Packet& p) {
            bool hasBad = false;
            for(auto& client : connections) {
                if (client && client->isConnected()){
                    client->send(p);
                }
                else{
                    onDisconnection(client);
                    client.reset();
                    hasBad = true;
                }
            }
            if(hasBad)
                 connections.erase(std::remove(connections.begin(), connections.end(), nullptr), connections.end());
        }

        void respond(size_t limit = -1) {
            size_t processed = 0;
            while(!inQueue.empty() && processed < limit){
                auto[p, client] = inQueue.front();
                inQueue.pop();
                onPacketReceive(client, p);
                processed++;
            }
        }
    protected:
        void waitForConnections() {
            serverAcceptor.async_accept([this](std::error_code ec, boost::asio::ip::tcp::socket socket){
                if(ec) {
                    std::cerr << "SERVER got an error whilst waiting for connections:\n\t" << ec.message() << "\n";
                }
                else{
                    std::cout << socket.remote_endpoint() << " connected to the SERVER\n";
                    std::shared_ptr<Connection> newConnection = std::make_shared<Connection>(Connection::Owner::SERVER,
                                serverContext, std::move(socket), inQueue);
                    if(onConnection(newConnection)) {
                        connections.push_back(std::move(newConnection));
                        connections.back()->connectToClient(IDs++);
                        std::cout << "SERVER has accepted connection" << connections.back()->getID() << "\n";
                    }
                    else{
                        std::cout << "SERVER has denied connection\n";
                    }
                }
                waitForConnections();
            });
        }
        uint32_t IDs = 0;
        virtual bool onConnection(std::shared_ptr<Connection> client) {
            return true;
        }
        virtual void onDisconnection(std::shared_ptr<Connection> client) {
        }
        virtual void onPacketReceive(std::shared_ptr<Connection> client, Packet p) {
        }

    };

}