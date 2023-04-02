#include "Event.h"
#include "IDmanager.h"
#include "Pawns/Pawn.h"
#include "Buildings/Building.h"
#include <stdexcept>


Event::Event(Event::Type t, int id) : p(Packet::Type::RAW_MESSAGE) {
	if(t != Event::Type::BUILDING_APPEAR &&
	   t != Event::Type::BUILDING_DISAPPEAR &&
	   t != Event::Type::PAWN_APPEAR &&
	   t != Event::Type::BUILDING_DISAPPEAR &&
	   t != Event::Type::PAWN_LET_RES)
		throw std::invalid_argument("Trying to make event with wrong type");
	std::vector<uint8_t> tmp(sizeof(t));
	std::memcpy(tmp.data(), &t, sizeof(t));
	if(t == Event::Type::BUILDING_APPEAR) {
		Building* bu = getBuilding(id);
		tmp += bu->serialize();
	}
	else if (t == Event::Type::PAWN_APPEAR) {
		Pawn* pw = getPawn(id);
		tmp += pw->serialize();
	}
	else{
		tmp.resize(sizeof(t) + sizeof(id));
		std::memcpy(tmp.data() + sizeof(t), &id, sizeof(id));
	}
	p << tmp;
}

Event::Event(Event::Type t, int id, Resource res) : p(Packet::Type::RAW_MESSAGE) {
	if(t != Event::Type::BUILDING_ADD_RES &&
	   t != Event::Type::BUILDING_REMOVE_RES &&
	   t != Event::Type::PAWN_TAKE_RES)
		throw std::invalid_argument("Trying to make event with wrong type");
	std::vector<uint8_t> tmp(sizeof(t) + sizeof(id) + sizeof(res));
	std::memcpy(tmp.data(), &t, sizeof(t));
	std::memcpy(tmp.data() + sizeof(t), &id, sizeof(id));
	std::memcpy(tmp.data() + sizeof(t) + sizeof(id), &res, sizeof(res));
	p << tmp;
}

Event::Event(Event::Type t, int id, std::pair<double, double> pos) : p(Packet::Type::RAW_MESSAGE) {
	if (t != Event::Type::PAWN_MOVE)
		throw std::invalid_argument("Trying to make event with wrong type");
	std::vector<uint8_t> tmp(sizeof(t) + sizeof(id) + sizeof(pos));
	std::memcpy(tmp.data(), &t, sizeof(t));
	std::memcpy(tmp.data() + sizeof(t), &id, sizeof(id));
	std::memcpy(tmp.data() + sizeof(t) + sizeof(id), &pos, sizeof(pos));
	p << tmp;
}

Packet Event::getPacket() {
	Packet result = p;
	result.prepare();
	return result;
}