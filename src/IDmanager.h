

#include <unordered_map>

class Entity;
class Pawn;
class Building;

struct IDmanager {
    static std::unordered_map<int, void*> all;

    static int nextID;
    static int newID();
    static int newObject(void* ptr);
    static void* get(int id);
    static void set(int id, void* data);
};

template<typename T>
class ptr{
    int id = -1;

    ptr(int id):id(id){}

    T operator*() {
        return *(reinterpret_cast<T*>(IDmanager::get(id)));
    }

    T* operator->() {
        return reinterpret_cast<T*>(IDmanager::get(id));
    }
};