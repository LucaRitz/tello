#pragma once

#include "connection_data.hpp"
#include <optional>
#include "udp_listener.hpp"
#include "../response/status_response.hpp"
#include <memory>

using tello::ConnectionData;
using std::optional;
using tello::UdpListener;
using tello::StatusResponse;
using std::unique_ptr;

namespace tello {

    class Command;

    class Network {
    public:
        Network() = delete;
        Network(const Network&) = delete;
        Network(Network&&) = delete;

        static bool connect();
        static void disconnect();

        static unique_ptr<Response> exec(const Command& command, const Tello& tello);

    private:
        static ConnectionData _commandConnection;
        static ConnectionData _statusConnection;
        static ConnectionData _videoConnection;
        static std::shared_mutex _connectionMutex;

        static StatusResponse statusResponseFactory(const char* const result);
        static void invokeStatusListener(const StatusResponse& response, const Tello& tello);

        static UdpListener<StatusResponse, statusResponseFactory, invokeStatusListener> _statusListener;

        static optional<ConnectionData> connectToPort(int port, const ConnectionData& connectionData);
    };
}