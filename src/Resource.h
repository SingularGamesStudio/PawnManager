#ifndef RESOURCE_H
#define RESOURCE_H
#include <iostream>
#ifdef SERVER_SIDE
#include "net/server.h"
dlib::ServerInterface* global_server = nullptr;
#endif
///misha does not believe in me  :(
enum class Resource : uint8_t { DummyOre, DummyIngot, DummyWeapon, DummyNothing };
#endif//RESOURCE_H