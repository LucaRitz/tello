#include <tello/connection/network.hpp>
#include <tello/logger/logger.hpp>
#include <tello/tello.hpp>
#include <tello/command.hpp>
#include "../response_factory.hpp"

#define COMMAND_PORT 8889
#define STATUS_PORT 8890
#define VIDEO_PORT 11111
#define BUFFER_LENGTH 1024

using tello::Logger;

ConnectionData tello::Network::_commandConnection{-1, {}};
ConnectionData tello::Network::_statusConnection = {-1, {}};
ConnectionData tello::Network::_videoConnection = {-1, {}};
std::shared_mutex tello::Network::_connectionMutex;
UdpListener<StatusResponse, tello::Network::statusResponseFactory, tello::Network::invokeStatusListener> tello::Network::_statusListener{
        _commandConnection, tello::Tello::_telloMapping, tello::Tello::_telloMappingMutex,
        tello::Network::_connectionMutex};

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
    optional<ConnectionData> command = Network::connectToPort(COMMAND_PORT, _commandConnection);
    optional<ConnectionData> status = Network::connectToPort(STATUS_PORT, _statusConnection);
    optional<ConnectionData> video = Network::connectToPort(VIDEO_PORT, _videoConnection);
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
    _connectionMutex.lock_shared();
    if (_commandConnection._fileDescriptor != -1) {
        closesocket(_commandConnection._fileDescriptor);
    }
    if (_statusConnection._fileDescriptor != -1) {
        closesocket(_statusConnection._fileDescriptor);
    }
    if (_videoConnection._fileDescriptor != -1) {
        closesocket(_videoConnection._fileDescriptor);
    }
    _connectionMutex.unlock_shared();

    WSACleanup();
}

unique_ptr<Response> tello::Network::exec(const Command& command, const Tello& tello) {
    string errorMessage = command.validate();
    if (!errorMessage.empty()) {
        Logger::get(LoggerType::COMMAND)->error(
                string("Command of type [{}] is not valid"), NAMES.find(command.type())->second);
        Logger::get(LoggerType::COMMAND)->error(string("Message is: {}"), errorMessage);
        return std::make_unique<Response>(Status::FAIL);
    }

    string commandString = command.build();

    char buffer[BUFFER_LENGTH];
    int n = 0;
    sockaddr_in sender{};
    memset(&sender, 0, sizeof(sender));

    _connectionMutex.lock_shared();
    int sendResult = sendto(_commandConnection._fileDescriptor, commandString.c_str(), commandString.length(),
           0, (const sockaddr*) &tello._clientaddr, sizeof(tello._clientaddr));
    if (sendResult == SOCKET_ERROR) {
        Logger::get(LoggerType::COMMAND)->info(
                string("Command of type [{}] is not sent cause of socket error!"), NAMES.find(command.type())->second);
        _connectionMutex.unlock_shared();
    } else {
        Logger::get(LoggerType::COMMAND)->info(
                string("Command of type [{}] is sent!"), NAMES.find(command.type())->second);
        int senderAddrSize = sizeof(sender);
        n = recvfrom(_commandConnection._fileDescriptor, (char*) buffer, BUFFER_LENGTH, MSG_WAITALL,
                     (struct sockaddr*) &sender, &senderAddrSize);
        _connectionMutex.unlock_shared();
    }

    n = n >= 0 ? n : 0;
    buffer[n] = '\0';

    string responseString(buffer);
    return ResponseFactory::build(command.type(), responseString);
}

optional<ConnectionData> tello::Network::connectToPort(int port, const ConnectionData& data) {
    if (data._fileDescriptor != -1) {
        Logger::get(LoggerType::COMMAND)->warn(string("Is already connected!"));
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
        Logger::get(LoggerType::COMMAND)->error(
                string("Bind failed. Port {0:d}"), port);
        return std::nullopt;
    }

    return std::make_optional<ConnectionData>(ConnectionData{fileDescriptor, servaddr});
}

StatusResponse tello::Network::statusResponseFactory(const char* const result) {
    return StatusResponse{string(result)};
}

void tello::Network::invokeStatusListener(const StatusResponse& response, const tello::Tello& tello) {
    if (tello._statusHandler != nullptr) {
        tello._statusHandler(response);
    }
}