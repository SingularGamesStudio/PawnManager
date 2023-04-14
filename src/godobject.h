//
// Created by ME on 14.04.2023.
//

#ifndef PAWNMANAGER_GODOBJECT_H
#define PAWNMANAGER_GODOBJECT_H

#include "net/server.h"
#ifdef SERVER_SIDE
struct godObject {
    static dlib::ServerInterface* global_server;
};
#endif

#endif//PAWNMANAGER_GODOBJECT_H
