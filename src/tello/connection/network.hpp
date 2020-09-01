#pragma once

#include <optional>
#include "udp_listener.hpp"
#include "tello/response/status_response.hpp"
#include <memory>
#include "tello/native/network_interface.hpp"
#include "tello/video_analyzer.hpp"
#include "../command/command.hpp"
#include "tello/logger/logger_interface.hpp"
#include "udp_command_listener.hpp"
#include "../thread/thread_pool.hpp"
#include <tello/tello.hpp>
#include <vector>

using tello::ConnectionData;
using std::optional;
using tello::UdpListener;
using tello::StatusResponse;
using std::shared_ptr;
using std::unique_ptr;
using tello::NetworkInterface;
using tello::VideoAnalyzer;
using tello::Command;
using tello::LoggerInterface;
using tello::UdpCommandListener;
using std::vector;
using tello::threading::Threadpool;

namespace tello {

    class Network {
    public:
        Network() = delete;
        Network(const Network&) = delete;
        Network(Network&&) = delete;

        static bool connect();
        static void disconnect();

        template<typename CommandResponse>
        static future<CommandResponse>
        exec(const Command& command, const Tello& tello);

        template<typename CommandResponse>
        static unordered_map<ip_address, future<CommandResponse>>
        exec(const Command& command, unordered_map<ip_address, const Tello*> tellos);

    private:
        static ConnectionData _commandConnection;
        static ConnectionData _statusConnection;
        static ConnectionData _videoConnection;
        static std::shared_mutex _connectionMutex;
        static shared_ptr<NetworkInterface> networkInterface;
        static VideoAnalyzer _videoAnalyzer;
        static UdpCommandListener _commandListener;
        static Threadpool _threadpool;

        static void invokeStatusListener(NetworkResponse& response, const Tello* tello);
        static void invokeVideoListener(NetworkResponse& response, const Tello* tello);

        static UdpListener<invokeStatusListener> _statusListener;
        static UdpListener<invokeVideoListener> _videoListener;

        static optional<ConnectionData>
        connectToPort(unsigned short port, const ConnectionData& connectionData, const LoggerType& loggerType);
        static void disconnect(ConnectionData& connectionData, const LoggerType& loggerType);
    };

    template<typename CommandResponse>
    future<CommandResponse>
    Network::exec(const Command& command, const Tello& tello) {
        unordered_map<ip_address, const Tello*> tellos{};
        ip_address telloAddress = tello._clientaddr._ip;
        tellos[telloAddress] = &tello;

        unordered_map<ip_address, future<CommandResponse>> answers = exec<CommandResponse>(command, tellos);
        auto answer = answers.find(telloAddress);
        return std::move(answer->second);
    }

    template<typename CommandResponse>
    unordered_map<ip_address, future<CommandResponse>>
    Network::exec(const Command& command, unordered_map<ip_address, const Tello*> tellos) {
        unordered_map<ip_address, future<CommandResponse>> responses{};
        string errorMessage = command.validate();
        if (!errorMessage.empty()) {
            LoggerInterface::error(LoggerType::COMMAND,
                    string("Command of type [{}] is not valid"), NAMES.find(command.type())->second);
            LoggerInterface::error(LoggerType::COMMAND, string("Message is: {}"), errorMessage);

            for (auto tello : tellos) {
                CommandResponse response = CommandResponse{Status::FAIL};
                promise<CommandResponse> errorProm{};
                errorProm.set_value(response);
                responses[tello.first] = std::move(errorProm.get_future());
            }

            return responses;
        }

        string commandString = command.build();

        for(auto tello = tellos.begin(); tello != tellos.end(); ++tello) {
            _connectionMutex.lock_shared();
            int result = networkInterface->send(_commandConnection._fileDescriptor, tello->second->_clientaddr,
                                                commandString);
            _connectionMutex.unlock_shared();
            if (result == SEND_ERROR_CODE) {
                LoggerInterface::info(LoggerType::COMMAND,
                        string("Command of type [{}] is not sent cause of socket error!"),
                        NAMES.find(command.type())->second);

                CommandResponse response = CommandResponse{Status::FAIL};
                promise<CommandResponse> errorProm{};
                errorProm.set_value(response);
                responses[tello->first] = std::move(errorProm.get_future());

                tellos.erase(tello->first);
            } else {
                LoggerInterface::info(LoggerType::COMMAND,
                        string("Command of type [{0}] is sent to {1}!"), NAMES.find(command.type())->second,
                        std::to_string(tello->second->_clientaddr._ip));
            }
        }

        if (command.hasResponse()) {
            vector<ip_address> openResponses{};
            openResponses.reserve(tellos.size());
            for(auto aTello : tellos) {
                openResponses.push_back(aTello.first);
            }
            unordered_map<ip_address, future<CommandResponse>> futures = _commandListener.append<CommandResponse>(openResponses);
            for(auto& future : futures) {
                responses[future.first] = std::move(future.second);
            }
        } else {
            for(auto aTello : tellos) {
                CommandResponse response = CommandResponse{Status::UNKNOWN};
                promise<CommandResponse> unknownProm{};
                unknownProm.set_value(response);
                responses[aTello.first] = std::move(unknownProm.get_future());
            }
        }

        return responses;
    }
}