#pragma once
#include "general.h"
#include "packet.h"
#include "mqueue.h"
#include "connection.h"

namespace dlib {

    class clientInterface {
    private:
        asio::io_context connectionContext;
        std::thread contextThread;
        asio::ip::tcp::socket connectionSocket;
        std::unique_ptr<Connection> connectionPtr;
        asio::ip::tcp::endpoint server;
    public:
        MQueue<std::pair<Packet, std::shared_ptr<Connection>>> inQueue;
        clientInterface() : connectionSocket(connectionContext) {}
        virtual ~clientInterface() {
            disconnect();
        }
        bool connect(const std::string& address, const uint16_t port) {
            try {
                asio::ip::tcp::resolver resolver(connectionContext);
                asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(address, std::to_string(port));
                connectionPtr = std::make_unique<Connection>(
                    Connection::Owner::CLIENT,
                    connectionContext,
                    asio::ip::tcp::socket(connectionContext),
                    inQueue
                );
                connectionPtr->connectToServer(endpoints);
                contextThread = std::thread([this]() { connectionContext.run(); });
            }
            catch(std::exception& e) {
                std::cerr << "Exception caught whilst trying to connect to server:\n\t" << e.what() << "\n";
                return false;
            }
            return false;
        }
        void disconnect() {
            if (isConnected())
                connectionPtr->disconnect();
            connectionContext.stop();
            if(contextThread.joinable())
                contextThread.join();
            connectionPtr->~Connection();
            connectionPtr.release();
        }
        bool isConnected() {
            if(!connectionPtr)
                return false;
            return connectionPtr->isConnected();
        }
        void send(const Packet& p) {
            connectionPtr->send(p);
        }
    }

}