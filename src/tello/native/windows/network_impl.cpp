#include "network_impl.hpp"
#include <tello/logger/logger.hpp>

#define BUFFER_LENGTH 2048

using tello::NetworkData;
using tello::NetworkResponse;
using tello::LoggerType;
using tello::Logger;
using tello::SIN_FAM;
using tello::ConnectionData;

tello::windows::NetworkImpl::NetworkImpl() : _wsaData(), _initializedConnection(false) {}

tello::windows::NetworkImpl::~NetworkImpl() {
    WSACleanup();
}

optional<ConnectionData> tello::windows::NetworkImpl::connect(const NetworkData& data, const LoggerType& logger) {
    if (!_initializedConnection) {
        int result = WSAStartup(MAKEWORD(2, 2), &_wsaData);
        if (result != 0) {
            Logger::get(logger)->info(string("Cannot initialize windows socket"));
            return std::nullopt;
        }
        _initializedConnection = true;
    }

    int fileDescriptor;
    sockaddr_in servaddr{};
    memset(&servaddr, 0, sizeof(servaddr));

    if ((fileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        Logger::get(LoggerType::COMMAND)->error(
                std::string("Socket creation failed. Port {0:d}"), data._port);
        return std::nullopt;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(data._port);

    if (bind(fileDescriptor, (const struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        Logger::get(logger)->error(
                string("Bind failed. Port {0:d}"), data._port);
        closesocket(fileDescriptor);
        return std::nullopt;
    }

    if (!setTimeout(fileDescriptor, 1000, logger)) {
        closesocket(fileDescriptor);
    };

    return std::make_optional<ConnectionData>(ConnectionData{fileDescriptor,
                                                             NetworkData{SIN_FAM::I_AF_INET,
                                                                         ntohs(servaddr.sin_port),
                                                                         ntohl(servaddr.sin_addr.s_addr)}});
}

bool tello::windows::NetworkImpl::disconnect(const int& fileDescriptor) {
    shutdown(fileDescriptor, SD_BOTH);
    int result = closesocket(fileDescriptor);
    return result != SOCKET_ERROR;
}

bool tello::windows::NetworkImpl::setTimeout(const int& fileDescriptor, const unsigned int timeout,
                                             const LoggerType& logger) {
    DWORD timeoutD = timeout;
    if (setsockopt(fileDescriptor, SOL_SOCKET, SO_RCVTIMEO,
                   (const char*) &timeoutD,
                   sizeof(DWORD)
    ) < 0) {
        Logger::get(logger)->error(
                std::string("Cannot set receive timeout of {0:d}"), timeout);
        return false;
    }
    return true;
}

int
tello::windows::NetworkImpl::send(const int& fileDescriptor, const NetworkData& receiver, const string& value) const {

    sockaddr_in target = map(receiver);


    int sendResult = sendto(fileDescriptor, value.c_str(), value.length(), 0, (const sockaddr*) &target,
                            sizeof(target));

    if (sendResult == SOCKET_ERROR) {
        return SEND_ERROR_CODE;
    }

    return 0;
}

NetworkResponse tello::windows::NetworkImpl::read(const int& fileDescriptor) const {
    char buffer[BUFFER_LENGTH];
    int n = 0;
    sockaddr_in sender{};
    memset(&sender, 0, sizeof(sender));
    int senderAddrSize = sizeof(sender);

    n = recvfrom(fileDescriptor, (char*) buffer, BUFFER_LENGTH, 0, (struct sockaddr*) &sender, &senderAddrSize);

    n = n >= 0 ? n : 0;
    buffer[n] = '\0';

    return NetworkResponse(NetworkData{SIN_FAM::I_AF_INET, ntohs(sender.sin_port), ntohl(sender.sin_addr.s_addr)},
                           string(buffer));
}

sockaddr_in tello::windows::NetworkImpl::map(const NetworkData& source) const {
    sockaddr_in sockAddr{};
    memset(&sockAddr, 0, sizeof(sockAddr));

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(source._port);
    sockAddr.sin_addr.S_un.S_addr = htonl(source._ip);

    return sockAddr;
}