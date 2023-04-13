#ifndef WORKERPAWN_H
#define WORKERPAWN_H
///misha does not believe in me  :(
#include <cstddef>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

#include "Pawn.h"

const uint8_t DummyWorker = 0xFF;

enum class expertisesID : uint8_t { DummySmeltery, DummyMetalworking, DummyTrainership };
class WorkerPawn : public Pawn {
public:
    size_t currentInWay;
    std::set<expertisesID> expertises;
    std::vector<ptr<Building>> onTheWay;
    WorkerPawn(int id) { this->id = id; }
    void create(ptr<Building> placeOfCreation);
    void assignTask(const Task& toAssign) override;
    void moveToBuilding(ptr<Building> dest) override;
    void moveToPosition(std::pair<double, double> toMove) override{};
    void tick(double deltaTime) override;
    virtual ~WorkerPawn();
    std::vector<uint8_t> serialize() const override;
    size_t deserialize(const std::vector<uint8_t>& data) override;
protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const std::vector<uint8_t> &data);
};
#endif//WORKERPAWN_H