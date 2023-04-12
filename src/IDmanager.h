#ifndef PAWNMANAGER_IDMANAGER_H
#define PAWNMANAGER_IDMANAGER_H

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

struct RequiresID {
    int id;
};

template<typename T>
struct ptr {
    int id = -1;
    ptr() : id(-1) {}

    ptr(int id) : id(id) {}

    explicit operator bool() const {
        if (id == -1 || IDmanager::get(id) == nullptr) return false;
        return true;
    }

    template<typename Q>
    Q* dyn_cast() const {
        return dynamic_cast<Q*>(reinterpret_cast<T*>(IDmanager::get(id)));
    }

    T operator*() const { return *(reinterpret_cast<T*>(IDmanager::get(id))); }

    T* operator->() const { return reinterpret_cast<T*>(IDmanager::get(id)); }

    bool operator<(const ptr& other) const { return id < other.id; }
    bool operator==(const ptr& other) const { return id == other.id; }

    void del() const {
        delete reinterpret_cast<T*>(IDmanager::get(id));
        IDmanager::set(id, nullptr);
    }

    T* pointer() const { return reinterpret_cast<T*>(IDmanager::get(id)); }

    template<typename Q>
    explicit operator ptr<Q>() const {
        return ptr<Q>(id);
    }
};

namespace std {
    template<typename T>
    struct hash<ptr<T>> {
        size_t operator()(const ptr<T>& p) const { return p.id; }
    };
}// namespace std

template<typename T, typename... Targs>
ptr<T> makeptr(Targs... args) {
    T* mem = reinterpret_cast<T*>(new char[sizeof(T)]);
    new (mem) T(args...);
    int id = IDmanager::newObject(mem);
    if (dynamic_cast<RequiresID*>(mem) != nullptr) { dynamic_cast<RequiresID*>(mem)->id = id; }
    return ptr<T>(id);
}

#endif