#include <tello/tello.hpp>
#include <tello/response.hpp>
#include <tello/command.hpp>
#include <tello/logger/logger.hpp>
#include "response_factory.hpp"
#include <tello/command_type.hpp>
#include <WinSock2.h>

#define DEFAULT_TELLO_IP_ADDRESS (ULONG)0xC0A80A01 // 192.168.10.1
#define COMMAND_PORT 8889
#define STATUS_PORT 8890
#define VIDEO_PORT 11111
#define BUFFER_LENGTH 1024

using tello::Response;
using tello::ConnectionData;
using tello::Logger;
using tello::Status;

const ConnectionData tello::Tello::_commandConnection = Tello::connectToPort(COMMAND_PORT);
const ConnectionData tello::Tello::_statusConnection = Tello::connectToPort(STATUS_PORT);
const ConnectionData tello::Tello::_videoConnection = Tello::connectToPort(VIDEO_PORT);
unordered_map<ip_address, const tello::Tello*> tello::Tello::_telloMapping{};
UdpListener<StatusResponse, tello::Tello::statusResponseFactory, tello::Tello::invokeStatusListener> tello::Tello::_statusListener{
        _commandConnection, _telloMapping};

tello::Tello::Tello(ip_address telloIp) : _clientaddr(sockaddrOf(telloIp)),
                                          _statusHandler(nullptr){
    _telloMapping[telloIp] = this;
}

tello::Tello::~Tello() {
    tello::Tello::_statusListener.stop();
    // TODO: Close connections
}

void tello::Tello::setStatusHandler(status_handler statusHandler) {
    this->_statusHandler = statusHandler;
}

unique_ptr<Response> tello::Tello::exec(const Command& command) {
    if (!_telloMapping.empty()) {
        return exec(command, _telloMapping.begin()->second->_clientaddr.sin_addr.s_addr);
    }
    return exec(command, DEFAULT_TELLO_IP_ADDRESS);
}

unique_ptr<Response> tello::Tello::exec(const Command& command, ip_address telloIp) {
    auto telloIt = _telloMapping.find(telloIp);
    if (telloIt == _telloMapping.end()) {
        Logger::get(LoggerType::COMMAND)->error(string("Tello with ip {0:x} not found"), telloIp);
        return std::make_unique<Response>(Status::FAIL);
    }

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
           0, (const sockaddr*) &telloIt->second->_clientaddr, len);
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

ConnectionData tello::Tello::connectToPort(int port) {
    int fileDescriptor;
    sockaddr_in servaddr{};
    memset(&servaddr, 0, sizeof(servaddr));

    if ((fileDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        Logger::get(LoggerType::COMMAND)->error(
                std::string("Socket creation failed. Port {0:d}"), port);
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    if (bind(fileDescriptor, (const struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        Logger::get(LoggerType::COMMAND)->error(
                string("Bind failed. Port ") + std::to_string(port));
        exit(EXIT_FAILURE);
    }

    return ConnectionData{fileDescriptor, servaddr};
}

sockaddr_in tello::Tello::sockaddrOf(ip_address telloIp) {
    sockaddr_in cliaddr{};
    memset(&cliaddr, 0, sizeof(cliaddr));

    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = telloIp;

    return cliaddr;
}

StatusResponse tello::Tello::statusResponseFactory(char* result) {
    return StatusResponse{string(result)};
}

void tello::Tello::invokeStatusListener(const StatusResponse& response, const tello::Tello& tello) {
    if (tello._statusHandler != nullptr) {
        tello._statusHandler(response);
    }
}