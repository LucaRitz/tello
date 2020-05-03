#pragma once

#include <optional>
#include "udp_listener.hpp"
#include "../response/status_response.hpp"
#include <memory>
#include "../native/network_interface.hpp"
#include "../video_analyzer.hpp"
#include "command_strategy.hpp"
#include "../command.hpp"
#include "../logger/logger.hpp"
#include "udp_command_listener.hpp"

using tello::ConnectionData;
using std::optional;
using tello::UdpListener;
using tello::StatusResponse;
using std::shared_ptr;
using std::unique_ptr;
using tello::NetworkInterface;
using tello::VideoAnalyzer;
using tello::CommandStrategy;
using tello::Command;
using tello::Logger;
using tello::UdpCommandListener;

#define SEND 3

namespace tello {

    class Network {
    public:
        Network() = delete;
        Network(const Network&) = delete;
        Network(Network&&) = delete;

        static bool connect();
        static void disconnect();

        template<typename CommandResponse, shared_ptr<CommandResponse> (* error)(), shared_ptr<CommandResponse>(* empty)()>
        static shared_ptr<CommandResponse>
        exec(const Command& command, const Tello& tello, const CommandStrategy& strategy);

        template<typename CommandResponse, shared_ptr<CommandResponse> (* error)(), shared_ptr<CommandResponse>(* empty)()>
        static unordered_map<ip_address, shared_ptr<CommandResponse>>
        exec(const Command& command, unordered_map<ip_address, const Tello*> tellos,
             const CommandStrategy& strategy);

    private:
        static ConnectionData _commandConnection;
        static ConnectionData _statusConnection;
        static ConnectionData _videoConnection;
        static std::shared_mutex _connectionMutex;
        static shared_ptr<NetworkInterface> networkInterface;
        static VideoAnalyzer _videoAnalyzer;
        static UdpCommandListener _commandListener;

        static StatusResponse statusResponseFactory(const NetworkResponse& networkResponse);
        static void invokeStatusListener(const StatusResponse& response, const Tello& tello);

        static string videoResponseFactory(const NetworkResponse& networkResponse);
        static void invokeVideoListener(const string& response, const Tello& tello);

        static UdpListener<StatusResponse, statusResponseFactory, invokeStatusListener> _statusListener;
        static UdpListener<string, videoResponseFactory, invokeVideoListener> _videoListener;

        static optional<ConnectionData>
        connectToPort(unsigned short port, const ConnectionData& connectionData, const LoggerType& loggerType);
        static void disconnect(ConnectionData& connectionData, const LoggerType& loggerType);
    };

    template<typename CommandResponse, shared_ptr<CommandResponse> (* error)(), shared_ptr<CommandResponse>(* empty)()>
    shared_ptr<CommandResponse>
    Network::exec(const Command& command, const Tello& tello, const CommandStrategy& strategy) {
        unordered_map<ip_address, const Tello*> tellos{};
        ip_address telloAddress = tello._clientaddr._ip;
        tellos[telloAddress] = &tello;

        unordered_map<ip_address, shared_ptr<CommandResponse>> answers = exec<CommandResponse, error, empty>(command, tellos, strategy);
        auto answer = answers.find(telloAddress);
        return std::move(answer->second);
    }

    template<typename CommandResponse, shared_ptr<CommandResponse> (* error)(), shared_ptr<CommandResponse>(* empty)()>
    unordered_map<ip_address, shared_ptr<CommandResponse>>
    Network::exec(const Command& command, unordered_map<ip_address, const Tello*> tellos,
                  const CommandStrategy& strategy) {
        unordered_map<ip_address, shared_ptr<CommandResponse>> responses{};
        string errorMessage = command.validate();
        if (!errorMessage.empty()) {
            Logger::get(LoggerType::COMMAND)->error(
                    string("Command of type [{}] is not valid"), NAMES.find(command.type())->second);
            Logger::get(LoggerType::COMMAND)->error(string("Message is: {}"), errorMessage);

            for (auto tello : tellos) {
                responses[tello.first] = error();
            }

            return responses;
        }

        string commandString = command.build();

        for(auto tello = tellos.begin(); tello != tellos.end(); ++tello) {
            int sendResult = SEND_ERROR_CODE;
            _connectionMutex.lock_shared();
            for (int i = 0; i < SEND; ++i) {
                int result = networkInterface->send(_commandConnection._fileDescriptor, tello->second->_clientaddr,
                                                    commandString);
                if (sendResult == SEND_ERROR_CODE) {
                    sendResult = result != SEND_ERROR_CODE ? result : SEND_ERROR_CODE;
                }
            }
            _connectionMutex.unlock_shared();
            if (sendResult == SEND_ERROR_CODE) {
                Logger::get(LoggerType::COMMAND)->info(
                        string("Command of type [{}] is not sent cause of socket error!"),
                        NAMES.find(command.type())->second);
                responses[tello->first] = error();
                tellos.erase(tello->first);
            } else {
                Logger::get(LoggerType::COMMAND)->info(
                        string("Command of type [{0}] is sent to {1:x}!"), NAMES.find(command.type())->second,
                        tello->second->_clientaddr._ip);
            }
        }

        for(auto aTello : tellos) {
            responses[aTello.first] = empty();
        }

        _commandListener.append(responses);

        return responses;
    }
}