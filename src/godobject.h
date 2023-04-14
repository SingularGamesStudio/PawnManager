//
// Created by ME on 14.04.2023.
//

#ifndef PAWNMANAGER_GODOBJECT_H
#define PAWNMANAGER_GODOBJECT_H

#include "LocalController.h"
#include "net/server.h"

struct godObject {
#ifdef SERVER_SIDE
    static dlib::ServerInterface* global_server;
#endif
#ifdef CLIENT_SIDE
    static LocalController* local_server;
#endif
    static size_t getRecipe(const uint8_t* data, Recipe*& out);
};


#endif//PAWNMANAGER_GODOBJECT_H
