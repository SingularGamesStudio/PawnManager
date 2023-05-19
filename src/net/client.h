#pragma once
#include "blocking_queue.h"
#include "connection.h"
#include "general.h"
#include "packet.h"

namespace dlib {

    class ClientInterface {
    private:
        boost::asio::io_context connectionContext;
        std::thread contextThread;
        std::unique_ptr<Connection> connectionPtr;
        boost::asio::ip::tcp::endpoint server;
        BlockingQueue<std::pair<Packet, std::shared_ptr<Connection>>> inQueue;

    public:
        ClientInterface() {}

        virtual ~ClientInterface() { disconnect(); }

        virtual void onPacketReceive(const Packet& p) {}

        bool connect(const std::string& address, const uint16_t port) {
            try {
                boost::asio::ip::tcp::resolver resolver(connectionContext);
                boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(address, std::to_string(port));
                connectionPtr = std::make_unique<Connection>(Connection::Owner::CLIENT, connectionContext,
                                                             boost::asio::ip::tcp::socket(connectionContext), inQueue);
                connectionPtr->connectToServer(endpoints);
                contextThread = std::thread([this]() { connectionContext.run(); });
            } catch (std::exception& e) {
                std::cerr << "Exception caught whilst trying to connect to server:\n\t" << e.what() << "\n";
                return false;
            }
            return false;
        }

        void disconnect() {
            if (isConnected()) connectionPtr->disconnect();
            connectionContext.stop();
            if (contextThread.joinable()) contextThread.join();
            connectionPtr->~Connection();
            connectionPtr.release();
        }

        bool isConnected() {
            if (!connectionPtr) return false;
            return connectionPtr->isConnected();
        }

        void send(const Packet& p) {
            if (isConnected()) connectionPtr->send(p);
        }

        template<typename Callback>
        void awaitPacket(Callback process, size_t count = 1) {
            while(count) {
                if(!inQueue.empty()) {
                    auto [p, client] = inQueue.front();
                    inQueue.pop();
                    process(p);
                    --count;
                }
            }
        }

        void respond(size_t limit = -1) {
            size_t processed = 0;
            while (!inQueue.empty() && processed < limit) {
                auto [p, client] = inQueue.front();
                inQueue.pop();
                onPacketReceive(p);
                processed++;
            }
        }
    };

}// namespace dlib