#include <tello/tello.hpp>
#include <tello/response.hpp>
#include <tello/command.hpp>
#include "response_factory.hpp"
#include <tello/connection/network.hpp>

#define COMMAND_PORT 8889

using tello::Response;
using tello::ConnectionData;
using tello::Logger;
using tello::Status;

unordered_map<ip_address, const tello::Tello*> tello::Tello::_telloMapping;
std::shared_mutex tello::Tello::_telloMappingMutex;

tello::Tello::Tello(ip_address telloIp) : _clientaddr(mapToNetworkData(telloIp)),
                                          _statusHandler(nullptr) {
    _telloMappingMutex.lock();
    _telloMapping[telloIp] = this;
    _telloMappingMutex.unlock();
}

tello::Tello::~Tello() {
    _telloMappingMutex.lock();
    _telloMapping.erase(_clientaddr._ip);
    _telloMappingMutex.unlock();
}

void tello::Tello::setStatusHandler(status_handler statusHandler) {
    this->_statusHandler = statusHandler;
}

void tello::Tello::setVideoHandler(video_handler videoHandler) {
    this->_videoHandler = videoHandler;
}

unique_ptr<Response> tello::Tello::exec(const Command& command) {
    return tello::Network::exec(command, *this);
}

ip_address tello::Tello::ip() const {
    return _clientaddr._ip;
}

NetworkData tello::Tello::mapToNetworkData(ip_address telloIp) {
    return NetworkData(SIN_FAM::I_AF_INET, COMMAND_PORT, telloIp);
}