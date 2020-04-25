#pragma once

#include <memory>
#include <unordered_map>
#include <WinSock2.h>

using std::unique_ptr;
using std::unordered_map;

using ip_address = ULONG;

namespace tello {

    class Command;
    class Response;

    struct ConnectionData {
        ConnectionData(int fileDescriptor, struct sockaddr_in servaddr);

        const int _fileDescriptor;
        const struct sockaddr_in _servaddr;
    };

    class Tello {
    public:
        explicit Tello(ip_address telloIp);
        ~Tello();

        static unique_ptr<Response> exec(const Command& command);
        static unique_ptr<Response> exec(const Command& command, ip_address telloIp);

    private:
        static const ConnectionData _commandConnection;
        static const ConnectionData _statusConnection;
        static const ConnectionData _videoConnection;
        static unordered_map<ip_address, const Tello*> _telloMapping;

        static ConnectionData connectToPort(int port);
        static sockaddr_in sockaddrOf(ip_address telloIp);

        const sockaddr_in _clientaddr;
    };
}