#pragma once

#include <optional>
#include "udp_listener.hpp"
#include "../response/status_response.hpp"
#include <memory>
#include "../native/network_interface.hpp"
#include "../video_analyzer.hpp"
#include "command_strategy.hpp"
#include "../command.hpp"
#include <tello/logger/logger.hpp>

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

#define SEND 3

namespace tello {

    class Network {
    public:
        Network() = delete;
        Network(const Network&) = delete;
        Network(Network&&) = delete;

        static bool connect();
        static void disconnect();

        template<typename CommandResponse, unique_ptr<CommandResponse> (* error)(), unique_ptr<CommandResponse> (* timeout)(), unique_ptr<CommandResponse>(* ofArg)(
                const string&)>
        static unique_ptr<CommandResponse>
        exec(const Command& command, const Tello& tello, const CommandStrategy& strategy);

        template<typename CommandResponse, unique_ptr<CommandResponse> (* error)(), unique_ptr<CommandResponse> (* timeout)(), unique_ptr<CommandResponse>(* ofArg)(
                const string&)>
        static unordered_map<ip_address, unique_ptr<CommandResponse>>
        exec(const Command& command, unordered_map<ip_address, const Tello*> tellos,
             const CommandStrategy& strategy);

    private:
        static ConnectionData _commandConnection;
        static ConnectionData _statusConnection;
        static ConnectionData _videoConnection;
        static std::shared_mutex _connectionMutex;
        static shared_ptr<NetworkInterface> networkInterface;
        static StatusResponse _statusResponse;
        static std::shared_mutex _statusMutex;
        static VideoAnalyzer _videoAnalyzer;

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

    template<typename CommandResponse, unique_ptr<CommandResponse> (* error)(), unique_ptr<CommandResponse> (* timeout)(), unique_ptr<CommandResponse>(* ofArg)(
            const string&)>
    unique_ptr<CommandResponse>
    Network::exec(const Command& command, const Tello& tello, const CommandStrategy& strategy) {
        unordered_map<ip_address, const Tello*> tellos{};
        ip_address telloAddress = tello._clientaddr._ip;
        tellos[telloAddress] = &tello;

        unordered_map<ip_address, unique_ptr<CommandResponse>> answers = exec<CommandResponse, error, timeout, ofArg>(command, tellos, strategy);
        auto answer = answers.find(telloAddress);
        return std::move(answer->second);
    }

    template<typename CommandResponse, unique_ptr<CommandResponse> (* error)(), unique_ptr<CommandResponse> (* timeout)(), unique_ptr<CommandResponse>(* ofArg)(
            const string&)>
    unordered_map<ip_address, unique_ptr<CommandResponse>>
    Network::exec(const Command& command, unordered_map<ip_address, const Tello*> tellos,
                  const CommandStrategy& strategy) {
        unordered_map<ip_address, unique_ptr<CommandResponse>> responses{};
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
        _statusMutex.lock_shared();
        t_forecast computedForecast = command.forecast(_statusResponse);
        _statusMutex.unlock_shared();

        _connectionMutex.lock_shared();
        networkInterface->setTimeout(_commandConnection._fileDescriptor, computedForecast, LoggerType::COMMAND);
        _connectionMutex.unlock_shared();

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

        //// COMMAND_AND_RETURN
        /*if(CommandStrategy::COMMAND_AND_RETURN == strategy) {
            for(auto& tello : tellos) {
                responses[tello.first] = ResponseFactory::unknown();
            }

            return responses;
        }*/

        //// COMMAND_AND_WAIT
        for(int i = 0; i < tellos.size();) {
            _connectionMutex.lock_shared();
            NetworkResponse networkResponse = networkInterface->read(_commandConnection._fileDescriptor);
            _connectionMutex.unlock_shared();

            ip_address senderIp = networkResponse._sender._ip;
            bool timeout = senderIp == 0;
            bool correctSender = tellos.find(senderIp) != tellos.end();
            bool ignore = false;

            if (timeout) {
                Logger::get(LoggerType::COMMAND)->error(
                        string("Timeout after command [{}]"), NAMES.find(command.type())->second);
            } else if (!correctSender) {
                Logger::get(LoggerType::COMMAND)->error(
                        string("Answer from wrong tello received {0:x}"), senderIp);
                ignore = true;
            } else {
                string answer = networkResponse._response;

                responses[senderIp] = ofArg(answer);
                if (Status::UNKNOWN != responses.find(senderIp)->second->status()) {
                    tellos.erase(senderIp);
                } else {
                    responses.erase(senderIp);
                    Logger::get(LoggerType::COMMAND)->warn(
                            string("Received unknown answer from {0:x}! (answer is ignored) -- {1}"), senderIp,
                            answer);
                    ignore = true;
                }
            }

            if(!ignore) {
                i++;
            }
        }

        for(auto aTello : tellos) {
            responses[aTello.first] = timeout();
        }

        return responses;
    }
}