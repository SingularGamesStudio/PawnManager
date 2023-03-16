#pragma once
#include "general.h"
#include "packet.h"
#include "mqueue.h"

namespace dlib {

    class Connection : public std::enable_shared_from_this<Connection> {
    public:
        enum class Owner : char {CLIENT, SERVER};
    protected:
        Owner owner;
        asio::io_context& context;
        asio::ip::tcp::socket soc;
        MQueue<Packet> sendQueue;
        MQueue<std::pair<Packet, std::shared_ptr<Connection>>>& receiveQueue;
        uint32_t id;
    private:
        Packet tmpPacket;
    public:

        Connection (Owner owner, asio::io_context& context, asio::ip::tcp::socket soc,
            MQueue<std::pair<Packet, std::shared_ptr<Connection>>>& receiveQueue) : owner(owner), context(context),
                soc(std::move(soc)), receiveQueue(receiveQueue)  {
        }
        virtual ~Connection () {
            disconnect();
        }

        uint32_t getID() const {
            return id;
        }

        void connectToClient(uint32_t clID) {
            if (owner != Owner::SERVER)
                return;
            if(soc.is_open()) {
                id = clID;
                acceptHeader();
            }
        }

        void connectToServer(const asio::ip::tcp::resolver::results_type& endpoints) {
            if (owner != Owner::CLIENT)
                return;
            asio::async_connect(soc, endpoints, [this](std::error_code ec, std::size_t len){
                if(ec){
                    std::cerr << "CLIENT failed to connectToServer. EROOR:\n\t" << ec.message() << "\n";
                    return;
                }
                acceptHeader();
            });
        }
        void disconnect() {
            if (isConnected()) 
                asio::post(context, [this]() { soc.close(); });
        }
        bool isConnected() const {
            return soc.is_open();
        }
        void send(const Packet& p) {
            asio::post(context, [this, p](){
                bool alreadySending = !sendQueue.empty();
                if(p.type == Packet::Type::PROCESSED_MESSAGE){
                    sendQueue.push(p);
                }
                else{
                    auto processedP = p;
                    processedP.prepare();
                    sendQueue.push(processedP);
                }
                if(!alreadySending)
                    sendPacket();
            });
        }

    private:
        void pushInQueue() {
            if(owner == Owner::SERVER)
                receiveQueue.push({tmpPacket, this->shared_from_this()});
            else
                receiveQueue.push({tmpPacket, nullptr});
            acceptHeader();
        }
        void acceptHeader() {
            char headerBuffer[Packet::headerSize];
            asio::async_read(soc, asio::buffer(headerBuffer, Packet::headerSize), 
                [this](std::error_code ec, std::size_t len){
                    if(ec){
                        std::cerr << "SERVER failed to read header ID: " << id << "\n";
                        tmpPacket.clear();
                        acceptHeader();
                        return;
                    }
                    tmpPacket.parseHeader(headerBuffer);
                    if(tmpPacket.size() > 0)
                        acceptData();
                    else
                        pushInQueue();
                });
        }

        void acceptData() {
            asio::async_read(soc, asio::buffer(tmpPacket.data.data(), tmpPacket.data.size()), 
                [this](std::error_code ec, std::size_t len){
                    if (ec) {
                        std::cerr << "SERVER failed to read data ID: " << id << "\n";
                        tmpPacket.clear();
                        acceptHeader();
                        return;
                    }
                    pushInQueue();
                });
        }

        void sendPacket() {
            asio::async_write(soc, asio::buffer(sendQueue.front().data.data(), sendQueue.front().data.size()),
                [this](std::error_code ec, std::size_t len){
                    if (ec) {
                        std::cerr << "SERVER failed to send packet ID: " << id << "\n";
                        return;
                    }
                    sendQueue.pop();
                    if (!sendQueue.empty()) {
                        sendPacket();
                    }
                });
        }
    }

}