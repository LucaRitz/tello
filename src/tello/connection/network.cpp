#include <tello/connection/network.hpp>
#include <tello/logger/logger.hpp>
#include <tello/tello.hpp>
#include <tello/command.hpp>
#include "../response_factory.hpp"
#include <tello/response.hpp>
#include "../native/network_interface_factory.hpp"

#define COMMAND_PORT 8889
#define STATUS_PORT 8890
#define VIDEO_PORT 11111
#define SEND 3

using tello::Logger;
using tello::Response;

ConnectionData tello::Network::_commandConnection{-1, {}};
ConnectionData tello::Network::_statusConnection = {-1, {}};
ConnectionData tello::Network::_videoConnection = {-1, {}};
std::shared_mutex tello::Network::_connectionMutex;
shared_ptr<NetworkInterface> tello::Network::networkInterface = tello::NetworkInterfaceFactory::build();
StatusResponse tello::Network::_statusResponse{""};
std::shared_mutex tello::Network::_statusMutex;
VideoAnalyzer tello::Network::_videoAnalyzer;
UdpListener<StatusResponse, tello::Network::statusResponseFactory, tello::Network::invokeStatusListener> tello::Network::_statusListener{
        _statusConnection, networkInterface, tello::Tello::_telloMapping, tello::Tello::_telloMappingMutex,
        tello::Network::_connectionMutex, LoggerType::STATUS};

UdpListener<string, tello::Network::videoResponseFactory, tello::Network::invokeVideoListener> tello::Network::_videoListener {
    _videoConnection, networkInterface, tello::Tello::_telloMapping, tello::Tello::_telloMappingMutex,
        tello::Network::_connectionMutex, LoggerType::VIDEO};

bool tello::Network::connect() {
    _connectionMutex.lock_shared();
    optional<ConnectionData> command = Network::connectToPort(COMMAND_PORT, _commandConnection, LoggerType::COMMAND);
    optional<ConnectionData> status = Network::connectToPort(STATUS_PORT, _statusConnection, LoggerType::STATUS);
    optional<ConnectionData> video = Network::connectToPort(VIDEO_PORT, _videoConnection, LoggerType::VIDEO);
    _connectionMutex.unlock_shared();

    bool isConnected = command && status && video;

    if (command) {
        _connectionMutex.lock();
        _commandConnection = command.value();
        _connectionMutex.unlock();
        Logger::get(LoggerType::COMMAND)->info(string("Command-Port connected"));
    } else {
        Logger::get(LoggerType::COMMAND)->info(string("Command-Port not connected"));
    }

    if (status) {
        _connectionMutex.lock();
        _statusConnection = status.value();
        _connectionMutex.unlock();
        Logger::get(LoggerType::STATUS)->info(string("Status-Port connected"));
    } else {
        Logger::get(LoggerType::STATUS)->info(string("Status-Port not connected"));
    }

    if (video) {
        _connectionMutex.lock();
        _videoConnection = video.value();
        _connectionMutex.unlock();
        Logger::get(LoggerType::VIDEO)->info(string("Video-Port connected"));
    } else {
        Logger::get(LoggerType::VIDEO)->info(string("Video-Port not connected"));
    }

    return isConnected;
}

void tello::Network::disconnect() {
    _statusListener.stop();
    _videoListener.stop();

    _connectionMutex.lock();
    if (_commandConnection._fileDescriptor != -1) {
        disconnect(_commandConnection, LoggerType::COMMAND);
    }
    if (_statusConnection._fileDescriptor != -1) {
        disconnect(_statusConnection, LoggerType::STATUS);
    }
    if (_videoConnection._fileDescriptor != -1) {
        disconnect(_videoConnection, LoggerType::VIDEO);
    }
    _connectionMutex.unlock();
}

unique_ptr<Response> tello::Network::exec(const Command& command, const Tello& tello) {
    unordered_map<ip_address, const Tello*> tellos{};
    ip_address telloAddress = tello._clientaddr._ip;
    tellos[telloAddress] = &tello;

    unordered_map<ip_address, unique_ptr<Response>> answers = exec(command, tellos);
    auto answer = answers.find(telloAddress);
    return std::move(answer->second);
}

unordered_map<ip_address, unique_ptr<Response>>
tello::Network::exec(const Command& command, unordered_map<ip_address, const Tello*> tellos) {
    unordered_map<ip_address, unique_ptr<Response>> responses{};
    string errorMessage = command.validate();
    if (!errorMessage.empty()) {
        Logger::get(LoggerType::COMMAND)->error(
                string("Command of type [{}] is not valid"), NAMES.find(command.type())->second);
        Logger::get(LoggerType::COMMAND)->error(string("Message is: {}"), errorMessage);

        for (auto tello : tellos) {
            responses[tello.first] = ResponseFactory::error();
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

    auto tello = tellos.begin();
    while (tello != tellos.end()) {
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
            responses[tello->first] = ResponseFactory::error();
            tellos.erase(tello->first);
        } else {
            Logger::get(LoggerType::COMMAND)->info(
                    string("Command of type [{0}] is sent to {1:x}!"), NAMES.find(command.type())->second,
                    tello->second->_clientaddr._ip);
        }

        ++tello;
    }

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
            responses[senderIp] = ResponseFactory::build(command.type(), answer);
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
        responses[aTello.first] = ResponseFactory::timeout();
    }

    return responses;
}

optional<ConnectionData>
tello::Network::connectToPort(unsigned short port, const ConnectionData& data, const LoggerType& loggerType) {
    if (data._fileDescriptor != -1) {
        Logger::get(loggerType)->warn(string("Is already connected!"));
        return std::make_optional<ConnectionData>(data);
    }

    return networkInterface->connect(NetworkData{SIN_FAM::I_AF_INET, port, 0}, loggerType);
}

void tello::Network::disconnect(ConnectionData& connectionData, const LoggerType& loggerType) {
    bool disconnected = networkInterface->disconnect(connectionData._fileDescriptor);
    if (disconnected) {
        Logger::get(loggerType)->info(string("Socket closed"));
        _commandConnection._fileDescriptor = -1;
    } else {
        Logger::get(loggerType)->error(string("Socket not closed"));
    }
}

StatusResponse tello::Network::statusResponseFactory(const NetworkResponse& networkResponse) {
    return StatusResponse{networkResponse._response};
}

void tello::Network::invokeStatusListener(const StatusResponse& response, const tello::Tello& tello) {
    _statusMutex.lock();
    _statusResponse = response;
    _statusMutex.unlock();
    if (tello._statusHandler != nullptr) {
        tello._statusHandler(response);
    }
}

string tello::Network::videoResponseFactory(const NetworkResponse& networkResponse) {
    return networkResponse._response;
}

void tello::Network::invokeVideoListener(const string& response, const Tello& tello) {
    _videoAnalyzer.append(response);

    if (_videoAnalyzer.finishedFrame()) {
        string frame = _videoAnalyzer.frame();
        _videoAnalyzer.clean();

        if (tello._videoHandler != nullptr) {
            tello._videoHandler(frame);
        }
    }
}