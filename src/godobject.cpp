//
// Created by ME on 14.04.2023.
//

#include "godobject.h"

#ifdef SERVER_SIDE
dlib::ServerInterface* godObject::global_server = nullptr;
#endif

#ifdef CLIENT_SIDE
LocalController* godObject::local_server = nullptr;
#endif