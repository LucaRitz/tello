#pragma once

#include <optional>
#include "udp_listener.hpp"
#include "../response/status_response.hpp"
#include <memory>
#include "../native/network_interface.hpp"

using tello::ConnectionData;
using std::optional;
using tello::UdpListener;
using tello::StatusResponse;
using std::shared_ptr;
using std::unique_ptr;
using tello::NetworkInterface;

namespace tello {

    class Command;
    class Response;

    class Network {
    public:
        Network() = delete;
        Network(const Network&) = delete;
        Network(Network&&) = delete;

        static bool connect();
        static void disconnect();

        static unique_ptr<Response> exec(const Command& command, const Tello& tello);
        static unordered_map<ip_address, unique_ptr<Response>>
        exec(const Command& command, unordered_map<ip_address, const Tello*> tellos);
    private:
        static ConnectionData _commandConnection;
        static ConnectionData _statusConnection;
        static ConnectionData _videoConnection;
        static std::shared_mutex _connectionMutex;
        static shared_ptr<NetworkInterface> networkInterface;

        static StatusResponse statusResponseFactory(const NetworkResponse& networkResponse);
        static void invokeStatusListener(const StatusResponse& response, const Tello& tello);

        static UdpListener<StatusResponse, statusResponseFactory, invokeStatusListener> _statusListener;

        static optional<ConnectionData>
        connectToPort(unsigned short port, const ConnectionData& connectionData, const LoggerType& loggerType);
        static void disconnect(ConnectionData& connectionData, const LoggerType& loggerType);
    };
}