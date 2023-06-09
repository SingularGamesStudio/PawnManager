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

enum class expertisesID : uint8_t { Nitwit, Lumbering, Mining, Smithing };
class WorkerPawn : public Pawn {
public:
    size_t currentInWay;
    std::set<expertisesID> expertises;
    std::vector<ptr<Building>> onTheWay;
#ifdef CLIENT_SIDE
    WorkerPawn() {}
#endif
    WorkerPawn(int id) { this->id = id; }
#ifdef SERVER_SIDE
    void create(ptr<Building> placeOfCreation);
    void assignTask(const Task& toAssign) override;
    void moveToBuilding(ptr<Building> dest);
    void moveToPosition(Position toMove) override{};
    void tick(double deltaTime) override;
#endif
    virtual ~WorkerPawn();

    std::vector<uint8_t> serialize() const override;
    std::vector<uint8_t> serializeExpertises() const;
    size_t updateExpertises(const uint8_t* data);
    size_t deserialize(const uint8_t* data) override;

protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const uint8_t* data);
};
#endif//WORKERPAWN_H