#include <tello/tello.hpp>
#include <tello/command.hpp>
#include <tello/connection/network.hpp>

#include "command/command_command.hpp"
#include "command/takeoff_command.hpp"
#include "command/land_command.hpp"
#include "command/streamon_command.hpp"
#include "command/streamoff_command.hpp"
#include "command/up_command.hpp"
#include "command/down_command.hpp"
#include "command/left_command.hpp"
#include "command/right_command.hpp"
#include "command/forward_command.hpp"
#include "command/back_command.hpp"
#include "command/clockwise_turn_command.hpp"
#include "command/counterclockwise_turn_command.hpp"
#include "command/flip_command.hpp"
#include "command/stop_command.hpp"
#include "command/emergency_command.hpp"
#include "command/set_speed_command.hpp"
#include "command/read_speed_command.hpp"
#include "command/read_wifi_command.hpp"

#define COMMAND_PORT 8889

using tello::Response;
using tello::ConnectionData;
using tello::Logger;
using tello::Status;

using namespace tello::command;

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

/////////////////////////////////////////////////////////////
///// COMMANDS //////////////////////////////////////////////
/////////////////////////////////////////////////////////////

future<Response> tello::Tello::command() const {
    const CommandCommand command;
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::takeoff() const {
    const TakeoffCommand command;
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::land() const {
    const LandCommand command;
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::streamon() const {
    const StreamOnCommand command;
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::streamoff() const {
    const StreamOffCommand command;
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::up(int x) const {
    const UpCommand command{ x };
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::down(int x) const {
    const DownCommand command{ x };
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::left(int x) const {
    const LeftCommand command{ x };
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::right(int x) const {
    const RightCommand command{ x };
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::forward(int x) const {
    const ForwardCommand command{ x };
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::back(int x) const {
    const BackCommand command{ x };
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::clockwise_turn(int x) const {
    const ClockwiseTurnCommand command{ x };
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::counterclockwise_turn(int x) const {
    const CounterclockwiseTurnCommand command{ x };
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::flip(char flip_direction) const
{
    const FlipCommand command{ flip_direction };
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::stop() const {
    const StopCommand command;
    return Network::exec<Response>(command, *this);
}

future<Response> tello::Tello::emergency() const {
    const EmergencyCommand command;
    return Network::exec<Response>(command, *this);
}


future<Response> tello::Tello::set_speed(int velocity) const {
    const SetSpeedCommand command { velocity };
    return Network::exec<Response>(command, *this);
}


future<QueryResponse> tello::Tello::read_speed() const {
    const ReadSpeedCommand command;
    return Network::exec<QueryResponse>(command, *this);
}

future<QueryResponse> tello::Tello::read_wifi() const {
    const ReadWifiCommand command;
    return Network::exec<QueryResponse>(command, *this);
}

/////////////////////////////////////////////////////////////
///// END COMMANDS //////////////////////////////////////////
/////////////////////////////////////////////////////////////

ip_address tello::Tello::ip() const {
    return _clientaddr._ip;
}

NetworkData tello::Tello::mapToNetworkData(ip_address telloIp) {
    return NetworkData(SIN_FAM::I_AF_INET, COMMAND_PORT, telloIp);
}