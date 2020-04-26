#pragma once

#include <WinSock2.h>

namespace tello {
    struct ConnectionData {
        ConnectionData(int fileDescriptor, struct sockaddr_in servaddr);

        int _fileDescriptor;
        struct sockaddr_in _servaddr;
    };
}