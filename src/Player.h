#ifndef PLAYER_H
#define PLAYER_H

#include <deque>
#include <vector>

#include "IDmanager.h"
#include "Recipes/Recipe.h"
#include "Task.h"

enum class expertisesID : uint8_t;
class Building;
class Pawn;
class Player;

class Player : public RequiresID {
public:
#ifdef CLIENT_SIDE
    Player(){}
#endif

    Player(int id) { this->id = id; }
    ptr<Building> hub;
    std::set<ptr<Pawn>> pawns;
#ifdef SERVER_SIDE
    struct TaskManager {

        struct PawnReq {
            virtual ptr<Pawn> find(ptr<Player> owner) = 0;
        };

        struct FighterReq : public PawnReq {
            FighterPawnType type;
            FighterReq(FighterPawnType type) : type(type) {}

            ptr<Pawn> find(ptr<Player> owner) override;
        };

        struct WorkerReq : public PawnReq {
            expertisesID expertise;
            WorkerReq(expertisesID expertise) : expertise(expertise) {}

            ptr<Pawn> find(ptr<Player> owner) override;
        };

        struct PendingRecipe {
            std::multiset<Resource> needResources;
            std::multiset<Resource> movedResources;
            std::multiset<Resource> doneResources;
            std::vector<PawnReq*> needPawns;
            std::set<ptr<Pawn>> movedPawns;
            std::unordered_map<ptr<Pawn>, PawnReq*> backupNeeds;
            std::vector<ptr<Pawn>> donePawns;

            int ID;
            int priority;
            Recipe* recipe;
            ptr<Building> place;

            bool operator<(const PendingRecipe& other) const { return ID < other.ID; }

            PendingRecipe(Recipe* recipe, ptr<Building> place, int priority);

            void start();

            ~PendingRecipe();
        };

        std::set<PendingRecipe*> work;

        ptr<Player> owner;

        void finishTask(Task task, ptr<Pawn> pawn);

        void cancelTask(Task task, ptr<Pawn> pawn);

        bool startRecipe(Recipe* recipe, ptr<Building> where);

        void tick();
    };

    TaskManager manager;

    bool checkRecipe(Recipe* recipe);


    ptr<CraftBuilding> placeBlueprint(std::pair<double, double> pos, ptr<Building> parent, double r);

    void tick();
    void attack(ptr<Building> what);

    ~Player();
#endif
#ifdef CLIENT_SIDE
    void localAttack(ptr<Building> what);
    void localStart(Recipe* recipe, ptr<Building> where);
#endif
    virtual std::vector<uint8_t> serialize() const;
    virtual size_t deserialize(const uint8_t* data);

protected:
    std::vector<uint8_t> serializeSelf() const;
    size_t deserializeSelf(const uint8_t* data);
};
#endif//PLAYER_H
