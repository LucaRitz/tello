#pragma once

#include <WinSock2.h>

namespace tello {
    struct ConnectionData {
        ConnectionData(int fileDescriptor, struct sockaddr_in servaddr);

        const int _fileDescriptor;
        const struct sockaddr_in _servaddr;
    };
}