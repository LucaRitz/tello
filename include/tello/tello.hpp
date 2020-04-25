#pragma once

#include <memory>
#include <unordered_map>
#include "connection/connection_data.hpp"
#include "connection/udp_listener.hpp"
#include "response/status_response.hpp"

using std::unique_ptr;
using std::unordered_map;
using tello::UdpListener;
using tello::StatusResponse;

using ip_address = unsigned long;

namespace tello {

    class Command;
    class Response;

    using status_handler = void (*)(const StatusResponse&);

    class Tello {
    public:
        explicit Tello(ip_address telloIp);
        ~Tello();

        void setStatusHandler(status_handler statusHandler);

        static unique_ptr<Response> exec(const Command& command);
        static unique_ptr<Response> exec(const Command& command, ip_address telloIp);

    private:
        static const ConnectionData _commandConnection;
        static const ConnectionData _statusConnection;
        static const ConnectionData _videoConnection;
        static unordered_map<ip_address, const Tello*> _telloMapping;

        static StatusResponse statusResponseFactory(char* result);
        static void invokeStatusListener(const StatusResponse& response, const Tello& tello);

        static UdpListener<StatusResponse, statusResponseFactory, invokeStatusListener> _statusListener;

        static ConnectionData connectToPort(int port);
        static sockaddr_in sockaddrOf(ip_address telloIp);

        const sockaddr_in _clientaddr;
        status_handler _statusHandler;
    };
}