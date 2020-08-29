#pragma once

#include <string>
#include <optional>
#include "../macro_definition.hpp"

#define SEND_ERROR_CODE -1

using ip_address = unsigned long;
using std::string;
using std::optional;

namespace tello {

    enum class LoggerType;

    enum class SIN_FAM {
        I_AF_INET
    };

    struct EXPORT NetworkData {
        NetworkData() = default;
        NetworkData(SIN_FAM sinFam, unsigned short port, ip_address ip);

        SIN_FAM _sinFam;
        unsigned short _port;
        ip_address _ip;
    };

    struct ConnectionData {
        ConnectionData(int fileDescriptor, NetworkData servaddr);

        int _fileDescriptor;
        NetworkData _networkData;
    };

    struct NetworkResponse {
        NetworkResponse(const NetworkData& sender, char* response, int size);
        NetworkResponse(const NetworkResponse& other);
        NetworkResponse& operator=(const NetworkResponse& other);
        NetworkResponse(NetworkResponse&& other) noexcept;
        NetworkResponse& operator=(NetworkResponse&& other) noexcept;
        ~NetworkResponse();

        [[nodiscard]] string response() const;

        NetworkData _sender;
        char* _response;
        int _length;
    };

    class NetworkInterface {
    public:
        virtual ~NetworkInterface() = default;

        virtual optional<ConnectionData> connect(const NetworkData& data, const LoggerType& logger) = 0;
        virtual bool disconnect(const int& fileDescriptor) = 0;
        virtual bool setTimeout(const int& fileDescriptor, const unsigned int timeout, const LoggerType& logger) = 0;

        [[nodiscard]] virtual int
        send(const int& fileDescriptor, const NetworkData& receiver, const string& value) const = 0;
        [[nodiscard]] virtual NetworkResponse read(const int& fileDescriptor) const = 0;
    };
}