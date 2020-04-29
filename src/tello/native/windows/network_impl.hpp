#pragma once
#include <WinSock2.h>
#include <tello/native/network_interface.hpp>

namespace tello::windows {

    class NetworkImpl : public NetworkInterface {
    public:
        NetworkImpl();
        ~NetworkImpl() override;

        optional<ConnectionData> connect(const NetworkData& data, const LoggerType& logger) override;
        bool disconnect(const int& fileDescriptor) override;

        [[nodiscard]] int
        send(const int& fileDescriptor, const NetworkData& receiver, const string& value) const override;
        [[nodiscard]] NetworkResponse read(const int& fileDescriptor) const override;

    private:
        WSAData _wsaData;
        bool _initializedConnection;

        [[nodiscard]] sockaddr_in map(const NetworkData& source) const;
    };
}