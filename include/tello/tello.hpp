#pragma once

#include <memory>
#include <unordered_map>
#include "connection/connection_data.hpp"
#include "response/status_response.hpp"
#include <shared_mutex>

using std::unique_ptr;
using std::unordered_map;
using tello::StatusResponse;

using ip_address = unsigned long;

namespace tello {

    class Command;
    class Response;
    class Network;

    using status_handler = void (*)(const StatusResponse&);

    class Tello {
    public:
        explicit Tello(ip_address telloIp);
        ~Tello();

        void setStatusHandler(status_handler statusHandler);
        unique_ptr<Response> exec(const Command& command);

        friend class Network;

    private:
        static unordered_map<ip_address, const Tello*> _telloMapping;
        static std::shared_mutex _telloMappingMutex;

        static sockaddr_in sockaddrOf(ip_address telloIp);

        const sockaddr_in _clientaddr;
        const ip_address _ip;
        status_handler _statusHandler;
    };
}