//
// Created by ME on 14.04.2023.
//

#include "LocalController.h"


void LocalController::onPacketReceive(const dlib::Packet &p) {
    Event::Type type = static_cast<Event::Type>(p.data[0]);
    std::vector<uint8_t> data = p.data;
    data.erase(data.begin());
    if(type==Event::Type::PLAYER_APPEAR) {

    }
}