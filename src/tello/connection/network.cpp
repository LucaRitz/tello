#include <tello/connection/network.hpp>
#include <tello/logger/logger.hpp>
#include <tello/tello.hpp>
#include <tello/command.hpp>
#include "../response_factory.hpp"
#include <WinSock2.h>

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

bool tello::Network::connect() {

    optional<ConnectionData> command = Network::connectToPort(COMMAND_PORT, _commandConnection);
    optional<ConnectionData> status = Network::connectToPort(STATUS_PORT, _statusConnection);
    optional<ConnectionData> video = Network::connectToPort(VIDEO_PORT, _videoConnection);

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

    int len = 0;
    sendto(_commandConnection._fileDescriptor, commandString.c_str(), commandString.length(),
           0, (const sockaddr*) &tello._clientaddr, len);
    Logger::get(LoggerType::COMMAND)->info(
            string("Command of type [{}] is sent!"), NAMES.find(command.type())->second);

    char buffer[BUFFER_LENGTH];
    int n;
    sockaddr_in sender{};
    memset(&sender, 0, sizeof(sender));

    n = recvfrom(_commandConnection._fileDescriptor, (char*) buffer, BUFFER_LENGTH, MSG_WAITALL,
                 (struct sockaddr*) &sender, &len);
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

    if ((fileDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
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