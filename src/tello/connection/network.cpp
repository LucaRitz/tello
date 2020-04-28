#include <tello/connection/network.hpp>
#include <tello/logger/logger.hpp>
#include <tello/tello.hpp>
#include <tello/command.hpp>
#include "../response_factory.hpp"
#include <tello/response.hpp>

#define COMMAND_PORT 8889
#define STATUS_PORT 8890
#define VIDEO_PORT 11111
#define BUFFER_LENGTH 1024
#define MAX_TRIES 5

using tello::Logger;
using tello::Response;

ConnectionData tello::Network::_commandConnection{-1, {}};
ConnectionData tello::Network::_statusConnection = {-1, {}};
ConnectionData tello::Network::_videoConnection = {-1, {}};
std::shared_mutex tello::Network::_connectionMutex;
UdpListener<StatusResponse, tello::Network::statusResponseFactory, tello::Network::invokeStatusListener> tello::Network::_statusListener{
        _commandConnection, tello::Tello::_telloMapping, tello::Tello::_telloMappingMutex,
        tello::Network::_connectionMutex, LoggerType::STATUS};

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
WSAData tello::Network::_wsaData;
bool tello::Network::_initializedConnection = false;
#endif

bool tello::Network::connect() {

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
        if (!_initializedConnection) {
            int result = WSAStartup(MAKEWORD(2,2), &_wsaData);
            if (result != 0) {
                Logger::get(LoggerType::COMMAND)->info(string("Cannot initialize windows socket"));
                return false;
            }
            _initializedConnection = true;
        }
    #endif

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
    // TODO: stop video listener

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

    WSACleanup();
}

unique_ptr<Response> tello::Network::exec(const Command& command, const Tello& tello) {
    unordered_map<ip_address, const Tello*> tellos{};
    ip_address telloAddress = tello._ip;
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

    char buffer[BUFFER_LENGTH];
    int n = 0;
    sockaddr_in sender{};
    memset(&sender, 0, sizeof(sender));

    int tries = 0;

    while (!tellos.empty() && tries < MAX_TRIES) {
        auto tello = tellos.begin();
        while (tello != tellos.end()) {
            _connectionMutex.lock_shared();
            int sendResult = sendto(_commandConnection._fileDescriptor, commandString.c_str(), commandString.length(),
                                    0, (const sockaddr*) &tello->second->_clientaddr,
                                    sizeof(tello->second->_clientaddr));
            _connectionMutex.unlock_shared();
            if (sendResult == SOCKET_ERROR) {
                Logger::get(LoggerType::COMMAND)->info(
                        string("Command of type [{}] is not sent cause of socket error!"),
                        NAMES.find(command.type())->second);
                responses[tello->first] = ResponseFactory::error();
                tellos.erase(tello->first);
            } else {
                Logger::get(LoggerType::COMMAND)->info(
                        string("Command of type [{0}] is sent to {1:x}!"), NAMES.find(command.type())->second,
                        tello->second->_ip);
            }

            ++tello;
        }

        bool timeout = false;
        int senderAddrSize = sizeof(sender);
        while(!timeout && !tellos.empty()) {
            _connectionMutex.lock_shared();
            n = recvfrom(_commandConnection._fileDescriptor, (char*) buffer, BUFFER_LENGTH, 0,
                         (struct sockaddr*) &sender, &senderAddrSize);
            _connectionMutex.unlock_shared();

            timeout = sender.sin_addr.s_addr == 0;
            ip_address senderIp = ntohl(sender.sin_addr.s_addr);
            bool correctSender = tellos.find(senderIp) != tellos.end();

            if (timeout) {
                Logger::get(LoggerType::COMMAND)->error(
                        string("Timeout after command [{}]"), NAMES.find(command.type())->second);
                tries++;
            } else if (!correctSender) {
                Logger::get(LoggerType::COMMAND)->error(
                        string("Answer from wrong tello received {0:x}"), senderIp);
            } else {
                n = n >= 0 ? n : 0;
                buffer[n] = '\0';

                string bufferedAnswer{buffer};
                if (bufferedAnswer.find("ok") != std::string::npos ||
                    bufferedAnswer.find("error") != std::string::npos) {
                    ip_address foundTelloIp = tellos.erase(senderIp);
                    responses[foundTelloIp] = ResponseFactory::build(command.type(), bufferedAnswer);
                } else {
                    Logger::get(LoggerType::COMMAND)->info(
                            string("Received unknown answer from {0:x}! (answer is ignored)"), senderIp);
                }
            }
        }
    }

    for(auto tello : tellos) {
        responses[tello.first] = ResponseFactory::timeout();
    }

    return responses;
}

optional<ConnectionData>
tello::Network::connectToPort(int port, const ConnectionData& data, const LoggerType& loggerType) {
    if (data._fileDescriptor != -1) {
        Logger::get(loggerType)->warn(string("Is already connected!"));
        return std::make_optional<ConnectionData>(data);
    }
    int fileDescriptor;
    sockaddr_in servaddr{};
    memset(&servaddr, 0, sizeof(servaddr));

    if ((fileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        Logger::get(LoggerType::COMMAND)->error(
                std::string("Socket creation failed. Port {0:d}"), port);
        return std::nullopt;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    if (bind(fileDescriptor, (const struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        Logger::get(loggerType)->error(
                string("Bind failed. Port {0:d}"), port);
        closesocket(fileDescriptor);
        return std::nullopt;
    }

    DWORD timeout = 2000;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeout;

    if (setsockopt(fileDescriptor, SOL_SOCKET, SO_RCVTIMEO,
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
                (const char*) &timeout,
                sizeof(DWORD)
        #else
                reinterpret_cast<char*>(&tv),
                sizeof(timeval) );
        #endif
    ) < 0) {
        Logger::get(loggerType)->error(
                std::string("Cannot set receive timeout of {0:d} on port {0:d}"), timeout, port);
        closesocket(fileDescriptor);
        return std::nullopt;
    }

    return std::make_optional<ConnectionData>(ConnectionData{fileDescriptor, servaddr});
}

void tello::Network::disconnect(ConnectionData& connectionData, const LoggerType& loggerType) {
    int result = closesocket(_commandConnection._fileDescriptor);
    if (result == SOCKET_ERROR) {
        Logger::get(loggerType)->error(string("Socket not closed"));
    } else {
        Logger::get(loggerType)->info(string("Socket closed"));
        _commandConnection._fileDescriptor = -1;
    }
}

StatusResponse tello::Network::statusResponseFactory(const char* const result) {
    return StatusResponse{string(result)};
}

void tello::Network::invokeStatusListener(const StatusResponse& response, const tello::Tello& tello) {
    if (tello._statusHandler != nullptr) {
        tello._statusHandler(response);
    }
}