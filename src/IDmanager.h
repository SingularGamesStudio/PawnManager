#ifndef PAWNMANAGER_IDMANAGER_H
#define PAWNMANAGER_IDMANAGER_H

#include <iostream>
#include <unordered_map>

class Entity;
class Pawn;
class Building;

struct RequiresID;

struct IDmanager {
    static std::unordered_map<int, RequiresID*> all;

    static int nextID;
    static int newID();
    static int newObject(RequiresID* ptr);
    static RequiresID* get(int id);
    static void set(int id, RequiresID* data);
};

struct RequiresID {
    int id;

    virtual ~RequiresID() { IDmanager::set(id, nullptr); }
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
    ptr<Q> dyn_cast() const {
        if (dynamic_cast<Q*>(IDmanager::get(id))) {
            return ptr<Q>(id);
        } else
            return ptr<Q>();
    }

    T operator*() const { return *(dynamic_cast<T*>(IDmanager::get(id))); }

    T* operator->() const { return dynamic_cast<T*>(IDmanager::get(id)); }

    bool operator<(const ptr& other) const { return id < other.id; }
    bool operator==(const ptr& other) const { return id == other.id; }

    void del() const {
        int id0 = id;
        delete dynamic_cast<T*>(IDmanager::get(id));
        IDmanager::set(id0, nullptr);
    }

    T* pointer() const { return dynamic_cast<T*>(IDmanager::get(id)); }

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
    int id = IDmanager::newID();
    new (mem) T(id, args...);
    IDmanager::set(id, dynamic_cast<RequiresID*>(mem));
    return ptr<T>(id);
}

#endif