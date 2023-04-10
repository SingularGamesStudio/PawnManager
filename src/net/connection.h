#pragma once
#include "general.h"
#include "mqueue.h"
#include "packet.h"

namespace dlib {

    class Connection : public std::enable_shared_from_this<Connection> {
    public:
        enum class Owner : char { CLIENT, SERVER };

    protected:
        Owner owner;
        boost::asio::io_context& context;
        boost::asio::ip::tcp::socket soc;
        MQueue<Packet> sendQueue;
        MQueue<std::pair<Packet, std::shared_ptr<Connection>>>& receiveQueue;
        uint32_t id;

    private:
        Packet tmpPacket;

    public:
        Connection(Owner owner, boost::asio::io_context& context, boost::asio::ip::tcp::socket soc,
                   MQueue<std::pair<Packet, std::shared_ptr<Connection>>>& receiveQueue)
            : owner(owner),
              context(context),
              soc(std::move(soc)),
              receiveQueue(receiveQueue),
              id(-1) {}
        virtual ~Connection() { disconnect(); }

        uint32_t getID() const { return id; }

        void connectToClient(uint32_t clID) {
            if (owner != Owner::SERVER) return;
            if (soc.is_open()) {
                id = clID;
                acceptHeader();
            }
        }

        void connectToServer(const boost::asio::ip::tcp::resolver::results_type& endpoints) {
            if (owner != Owner::CLIENT) return;
            boost::asio::async_connect(soc, endpoints, [this](std::error_code ec, boost::asio::ip::tcp::endpoint endpoint) {
                if (!ec) { acceptHeader(); }
            });
        }
        void disconnect() {
            if (isConnected()) boost::asio::post(context, [this]() { soc.close(); });
        }
        bool isConnected() const { return soc.is_open(); }
        void send(const Packet& p) {
            boost::asio::post(context, [this, p]() {
                bool alreadySending = !sendQueue.empty();
                if (p.type == Packet::Type::PROCESSED_MESSAGE) {
                    sendQueue.push(p);
                } else {
                    auto processedP = p;
                    processedP.prepare();
                    sendQueue.push(processedP);
                }
                if (!alreadySending) sendPacket();
            });
        }

    private:
        void pushInQueue() {
            if (owner == Owner::SERVER) receiveQueue.push({tmpPacket, this->shared_from_this()});
            else
                receiveQueue.push({tmpPacket, nullptr});
            tmpPacket.clear();
            acceptHeader();
        }
        void acceptHeader() {
            boost::asio::mutable_buffer headerBuffer(new char[9], 9);
            boost::asio::async_read(soc, headerBuffer, [this, headerBuffer](std::error_code ec, size_t len) {
                if (ec) {
                    std::cerr << "failed to read header ID: " << id << "\n";
                    tmpPacket.clear();
                    acceptHeader();
                    return;
                }
                tmpPacket.parseHeader((uint8_t*) headerBuffer.data());
                delete[] (uint8_t*) headerBuffer.data();
                if (!tmpPacket.data.empty()) acceptData();
                else
                    pushInQueue();
            });
        }

        void acceptData() {
            boost::asio::async_read(soc, boost::asio::buffer(tmpPacket.data.data(), tmpPacket.data.size()),
                                    [this](std::error_code ec, std::size_t len) {
                                        if (ec) {
                                            std::cerr << "failed to read data ID: " << id << "\n";
                                            tmpPacket.clear();
                                            acceptHeader();
                                            return;
                                        }
                                        pushInQueue();
                                    });
        }

        void sendPacket() {
            boost::asio::async_write(soc, boost::asio::buffer(sendQueue.front().data.data(), sendQueue.front().data.size()),
                                     [this](std::error_code ec, size_t len) {
                                         if (ec) {
                                             std::cerr << "SERVER failed to send packet ID: " << id << "\n";
                                             return;
                                         }
                                         sendQueue.pop();
                                         if (!sendQueue.empty()) { sendPacket(); }
                                     });
        }
    };

}// namespace dlib