#ifndef PLAYER_H
#define PLAYER_H

#include "Recipes/Recipe.h"
#include "Task.h"
#include <vector>
#include <deque>

enum class expertisesID;
class Building;
class Pawn;
class Player;

class Player:public RequiresID {
public:
    Player(){}
    Building* hub;
    std::vector<Pawn*> pawns;

    struct TaskManager {

        struct PawnReq {
            virtual Pawn* find(Player* owner) = 0;
        };

        struct FighterReq:public PawnReq {
            FighterPawnType type;
            FighterReq(FighterPawnType type) : type(type){}

            Pawn * find(Player *owner) override {};
        };

        struct WorkerReq:public PawnReq {
            expertisesID expertise;
            WorkerReq(expertisesID expertise):expertise(expertise){}

            Pawn * find(Player *owner) override {};
        };

        struct PendingRecipe {
            std::multiset<Resource> needResources;
            std::multiset<Resource> movedResources;
            std::multiset<Resource> doneResources;
            std::vector<PawnReq*> needPawns;
            std::vector<Pawn*> movedPawns;
            std::vector<Pawn*> donePawns;

            int ID;
            int priority;
            Recipe* recipe;
            Building* place;

            bool operator<(const PendingRecipe& other) const {
                return ID<other.ID;
            }

            PendingRecipe(Recipe* recipe, Building* place, int priority);

            void start();

            ~PendingRecipe();
        };

        std::set<PendingRecipe*> work;

        Player* owner;

        void finishTask(Task task, Pawn* pawn);

        void cancelTask(Task task, Pawn* pawn);

        bool startRecipe(Recipe* recipe, Building* where);

        void tick();
    };

    TaskManager manager;

    bool checkRecipe(Recipe* recipe);

    CraftBuilding* placeBlueprint(std::pair<double, double> pos, Building* parent, double r);

    void tick();
};
#endif //PLAYER_H
