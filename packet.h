#pragma once
#include "general.h"

namespace dlib {

    class Packet {
        static const size_t headerSize = sizeof(Type) + sizeof(size_t);
    public:
        enum class Type : char { INVALID, RAW_MESSAGE, PROCESSED_MESSAGE, ACCEPTED_MESSAGE };
        Type type;
        std::vector<char> data;
        Packet() : type(Type::INVALID), data() {}
        explicit Packet(Type type) : type(type), data() {}
        void parseHeader(char* rawData) {
            size_t size = 0;
            std::memcpy(&size, rawData, sizeof(size_t));
            std::memcpy(&type, rawData + sizeof(size_t), sizeof(Type));
            if(type != Type::PROCESSED_MESSAGE){
                type = Type::INVALID;
                throw std::logic_error("ERROR: received packet is not PROCESSED_MESSAGE type");
            }
            type = Type::ACCEPTED_MESSAGE;
            data.resize(size);
        }
        template<typename U>
        Packet& operator<<(const U& object) { // adds U to the packet, U has to be trivial type
            static_assert(std::is_standard_layout<U>::value, "U is not trivial type");
            if (type != Type::RAW_MESSAGE)
                throw std::logic_error("ERROR: trying to add data(U) into not RAW_MESSAGE packet");
            auto size = data.size();
            data.resize(size + sizeof(U));
            std::memcpy(data.data() + size, &object, sizeof(U));
            return *this;
        }
        template<>
        Packet& operator<<<std::vector<char>>(const std::vector<char>& object) { // adds vector of bytes to packet
            if (type != Type::RAW_MESSAGE)
                throw std::logic_error("ERROR: trying to add data(byte vector) into not RAW_MESSAGE packet");
            auto size = data.size();
            data.resize(size + object.size());
            std::memcpy(data.data() + size, object.data(), object.size());
            return *this;
        }
        Packet& operator+=(const Packet& that) { // adds contains of one RAW_MESSAGE packet into another
            if (type != Type::RAW_MESSAGE)
                throw std::logic_error("ERROR: trying to add data(another packet) into not RAW_MESSAGE packet");
            if (that.type != Type::RAW_MESSAGE)
                throw std::invalid_argument("ERROR: trying to add not RAW_MESSAGE packet");
            auto size = data.size();
            auto t_size = that.data.size();
            data.resize(size + t_size);
            std::memcpy(data.data() + size, that.data.data(), t_size);
            return *this;
        }
        void prepare() {
            if (type == Type::PROCESSED_MESSAGE)
                return;
            if (type != Type::RAW_MESSAGE)
                throw std::invalid_argument("ERROR: trying to prepare for sending unsendable packet");
            if (type == Type::RAW_MESSAGE)
                type = Type::PROCESSED_MESSAGE;
            auto size = data.size() + sizeof(Type);
            auto bytes = data.size();
            std::vector<char> tmp_header(headerSize);
            std::memcpy(tmp_header.data(), &bytes, sizeof(size_t));
            std::memcpy(tmp_header.data() + sizeof(size_t), &type, sizeof(Type));
            tmp_header.insert(tmp_header.end(), data.begin(), data.end());
            data = tmp_header;
        }
        void clear(Type newType = Type::INVALID) {
            type = newType;
            data.clear();
        }
    };

}
