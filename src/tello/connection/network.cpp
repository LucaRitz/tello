#include <tello/connection/network.hpp>
#include <tello/tello.hpp>
#include "../native/network_interface_factory.hpp"

#define COMMAND_PORT 8889
#define STATUS_PORT 8890
#define VIDEO_PORT 11111

using tello::Response;
using tello::NetworkResponse;

ConnectionData tello::Network::_commandConnection{-1, {}};
ConnectionData tello::Network::_statusConnection = {-1, {}};
ConnectionData tello::Network::_videoConnection = {-1, {}};
std::shared_mutex tello::Network::_connectionMutex;
shared_ptr<NetworkInterface> tello::Network::networkInterface = tello::NetworkInterfaceFactory::build();
VideoAnalyzer tello::Network::_videoAnalyzer;
UdpCommandListener tello::Network::_commandListener{_commandConnection, networkInterface, _connectionMutex};
UdpListener<StatusResponse, tello::Network::statusResponseFactory, tello::Network::invokeStatusListener> tello::Network::_statusListener{
        _statusConnection, networkInterface, tello::Tello::_telloMapping, tello::Tello::_telloMappingMutex,
        tello::Network::_connectionMutex, LoggerType::STATUS};

UdpListener<NetworkResponse, tello::Network::videoResponseFactory, tello::Network::invokeVideoListener> tello::Network::_videoListener {
    _videoConnection, networkInterface, tello::Tello::_telloMapping, tello::Tello::_telloMappingMutex,
        tello::Network::_connectionMutex, LoggerType::VIDEO};

bool tello::Network::connect() {
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
    _videoListener.stop();
    _commandListener.stop();

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
}

optional<ConnectionData>
tello::Network::connectToPort(unsigned short port, const ConnectionData& data, const LoggerType& loggerType) {
    if (data._fileDescriptor != -1) {
        Logger::get(loggerType)->warn(string("Is already connected!"));
        return std::make_optional<ConnectionData>(data);
    }

    return networkInterface->connect(NetworkData{SIN_FAM::I_AF_INET, port, 0}, loggerType);
}

void tello::Network::disconnect(ConnectionData& connectionData, const LoggerType& loggerType) {
    bool disconnected = networkInterface->disconnect(connectionData._fileDescriptor);
    if (disconnected) {
        Logger::get(loggerType)->info(string("Socket closed"));
        _commandConnection._fileDescriptor = -1;
    } else {
        Logger::get(loggerType)->error(string("Socket not closed"));
    }
}

StatusResponse tello::Network::statusResponseFactory(const NetworkResponse& networkResponse) {
    return StatusResponse{networkResponse.response()};
}

void tello::Network::invokeStatusListener(const StatusResponse& response, const tello::Tello& tello) {
    if (tello._statusHandler != nullptr) {
        tello._statusHandler(response);
    }
}

NetworkResponse tello::Network::videoResponseFactory(const NetworkResponse& networkResponse) {
    return networkResponse;
}

void tello::Network::invokeVideoListener(const NetworkResponse& response, const Tello& tello) {
    bool frameFull = _videoAnalyzer.append(response._response, response._length);

    if (frameFull) {
        unsigned char* frame = _videoAnalyzer.frame();
        unsigned int length = _videoAnalyzer.length();
        _videoAnalyzer.clean();

        if (tello._videoHandler != nullptr) {
            VideoResponse videoResponse = VideoResponse{frame, length};
            tello._videoHandler(videoResponse);
        }
    }
}