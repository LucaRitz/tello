#include <tello/commander.hpp>
#include <tello/response.hpp>
#include <tello/command.hpp>
#include <tello/logger/logger.hpp>
#include "response_factory.hpp"
#include <tello/command_type.hpp>

#define TELLO_IP_ADDRESS (ULONG)0xC0A80A01 // 192.168.10.1
#define COMMAND_RECEIVE_PORT 8889
#define STATE_PORT 8890
#define VIDEO_PORT 11111
#define BUFFER_LENGTH 1024

using tello::Response;
using tello::ConnectionData;
using tello::Logger;
using tello::Status;

tello::ConnectionData::ConnectionData(int fileDescriptor, struct sockaddr_in servaddr, struct sockaddr_in cliaddr) :
        fileDescriptor(fileDescriptor), servaddr(servaddr), cliaddr(cliaddr)
        {}

tello::Commander::Commander() : _defaultConnection(init(COMMAND_RECEIVE_PORT)) {

}

tello::Commander::~Commander() {

}

unique_ptr<Response> tello::Commander::exec(const Command& command) const {
    string errorMessage = command.validate();
    if (!errorMessage.empty()) {
        Logger::instance().get(LoggerType::DEFAULT)->warn(
                string("Command of type [") + NAMES.find(command.type())->second + string("] is not valid!"));
        Logger::instance().get(LoggerType::DEFAULT)->warn(
                string("Message is: ") + errorMessage);
        return std::make_unique<Response>(Status::FAIL);
    }

    string commandString = command.build();

    int len = 0;
    sendto(_defaultConnection.fileDescriptor, commandString.c_str(), commandString.length(),
           0, (const sockaddr*) &_defaultConnection.cliaddr, len);
    Logger::instance().get(LoggerType::DEFAULT)->info(
            string("Command of type [") + NAMES.find(command.type())->second + string("] is sent!"));

    char buffer[BUFFER_LENGTH];
    int n;
    n = recvfrom(_defaultConnection.fileDescriptor, (char*) buffer, BUFFER_LENGTH, MSG_WAITALL,
                 (struct sockaddr*) &_defaultConnection.cliaddr, &len);
    buffer[n] = '\0';

    string responseString(buffer);
    return ResponseFactory::build(command.type(), responseString);
}

ConnectionData tello::Commander::init(int port) {
    int fileDescriptor;
    sockaddr_in servaddr, cliaddr;

    if ((fileDescriptor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        Logger::instance().get(LoggerType::DEFAULT)->error(
                string("Socket creation failed. Port ") + std::to_string(port));
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = TELLO_IP_ADDRESS;
    cliaddr.sin_port = htons(port);

    if (bind(fileDescriptor, (const struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        Logger::instance().get(LoggerType::DEFAULT)->error(
                string("Bind failed. Port ") + std::to_string(port));
        exit(EXIT_FAILURE);
    }

    return ConnectionData{fileDescriptor, servaddr, cliaddr};
}

const tello::Commander& tello::Commander::instance() {
    static Commander instance;
    return instance;
}