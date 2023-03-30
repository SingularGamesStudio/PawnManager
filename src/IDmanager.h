

#include <unordered_map>

class Entity;
class Pawn;
class Building;

struct IDmanager {
    static std::unordered_map<int, Entity*> all;

    static int nextID;
    static int newEntity(Entity* entity);
    static int newID();
    static Pawn* getPawn(int id);
    static Building* getBuilding(int id);
};