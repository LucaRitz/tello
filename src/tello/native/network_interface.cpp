#include "tello/native/network_interface.hpp"

tello::NetworkData::NetworkData(const tello::SIN_FAM sinFam, const unsigned short port, const ip_address ip) :
        _sinFam(sinFam),
        _port(port),
        _ip(ip) {}

tello::ConnectionData::ConnectionData(int fileDescriptor, NetworkData networkData) :
        _fileDescriptor(fileDescriptor),
        _networkData(networkData) {};

tello::NetworkResponse::NetworkResponse(const tello::NetworkData& sender, string response) :
        _sender(sender),
        _response(std::move(response)) {}