#pragma once

#include <memory>
#include <WinSock2.h>

using std::unique_ptr;

namespace tello {

    class Command;
    class Response;

    struct ConnectionData {
        int fileDescriptor;
        struct sockaddr_in servaddr, cliaddr;
    };

    class Commander {
    public:
        static const Commander& instance();
        ~Commander();

        unique_ptr<Response> exec(const Command& command);

    private:
        Commander();
        static ConnectionData init(int port);

        const ConnectionData _defaultConnection;
    };
}