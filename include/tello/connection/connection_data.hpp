#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
    #include <WinSock2.h>
#endif

namespace tello {
    struct ConnectionData {
        ConnectionData(int fileDescriptor, struct sockaddr_in servaddr);

        int _fileDescriptor;
        struct sockaddr_in _servaddr;
    };
}