#pragma once
#include "general.h"

namespace dlib {

    class Packet {
    public:
        enum class Type : uint8_t { INVALID, RAW_MESSAGE, PROCESSED_MESSAGE, ACCEPTED_MESSAGE };
        static const size_t headerSize = sizeof(Type) + sizeof(size_t);
        Type type;
        std::vector<uint8_t> data;
        Packet() : type(Type::INVALID), data() {}
        ~Packet() = default;
        explicit Packet(Type type) : type(type), data() {}
        void parseHeader(const uint8_t* rawData) {
            size_t size = 0;
            std::memcpy(&size, rawData, sizeof(size_t));
            std::memcpy(&type, rawData + sizeof(size_t), sizeof(Type));
            if (type != Type::PROCESSED_MESSAGE) {
                type = Type::INVALID;
                throw std::logic_error("ERROR: received packet is not PROCESSED_MESSAGE type");
            }
            type = Type::ACCEPTED_MESSAGE;
            data.resize(size);
        }
        Packet& operator<<(const std::vector<uint8_t>& object) {// adds U to the packet, U has to be trivial type
            if (type != Type::RAW_MESSAGE) {
                throw std::logic_error("ERROR: trying to add data(U) into not RAW_MESSAGE packet");
                6;
            }
            size_t size = data.size();
            data.resize(size + object.size());
            std::memcpy(data.data() + size, object.data(), object.size());
            return *this;
        }
        Packet& operator+=(const Packet& that) {// adds contains of one RAW_MESSAGE packet into another
            if (type != Type::RAW_MESSAGE) throw std::logic_error("ERROR: trying to add data(another packet) into not RAW_MESSAGE packet");
            if (that.type != Type::RAW_MESSAGE) throw std::invalid_argument("ERROR: trying to add not RAW_MESSAGE packet");
            size_t size = data.size();
            size_t tSize = that.data.size();
            data.resize(size + tSize);
            std::memcpy(data.data() + size, that.data.data(), tSize);
            return *this;
        }
        void prepare() {
            if (type == Type::PROCESSED_MESSAGE) return;
            if (type != Type::RAW_MESSAGE) throw std::invalid_argument("ERROR: trying to prepare for sending unsendable packet");
            if (type == Type::RAW_MESSAGE) type = Type::PROCESSED_MESSAGE;
            size_t bytes = data.size();
            std::vector<uint8_t> tmpHeader(headerSize);
            std::memcpy(tmpHeader.data(), &bytes, sizeof(size_t));
            std::memcpy(tmpHeader.data() + sizeof(size_t), &type, sizeof(Type));
            tmpHeader.insert(tmpHeader.end(), data.begin(), data.end());
            data = tmpHeader;
        }
        void clear(Type newType = Type::INVALID) {
            type = newType;
            data.clear();
        }
    };

}// namespace dlib
