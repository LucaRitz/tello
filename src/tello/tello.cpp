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
#include "command/wifi_command.hpp"

#define COMMAND_PORT 8889

using tello::Response;
using tello::ConnectionData;
using tello::Logger;
using tello::Status;

using namespace tello::command;

unordered_map<ip_address, const tello::Tello*> tello::Tello::_telloMapping;
std::shared_mutex tello::Tello::_telloMappingMutex;

tello::Tello::Tello(ip_address telloIp) : TelloInterface(CommandStrategy::COMMAND_AND_WAIT),
                                          _clientaddr(mapToNetworkData(telloIp)),
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

unique_ptr<Response> tello::Tello::command() const {
    const CommandCommand command;
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, *this, _strategy);
}

unique_ptr<Response> tello::Tello::takeoff() const {
    TakeoffCommand command;
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, *this, _strategy);
}

unique_ptr<Response> tello::Tello::land() const {
    LandCommand command;
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, *this, _strategy);
}

unique_ptr<Response> tello::Tello::streamon() const {
    StreamOnCommand command;
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, *this, _strategy);
}

unique_ptr<Response> tello::Tello::streamoff() const {
    StreamOffCommand command;
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, *this, _strategy);
}

unique_ptr<Response> tello::Tello::up(int x) const {
    UpCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, *this, _strategy);
}

unique_ptr<Response> tello::Tello::down(int x) const {
    DownCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, *this, _strategy);
}

unique_ptr<Response> tello::Tello::left(int x) const {
    LeftCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, *this, _strategy);
}

unique_ptr<Response> tello::Tello::right(int x) const {
    RightCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, *this, _strategy);
}

unique_ptr<Response> tello::Tello::forward(int x) const {
    ForwardCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, *this, _strategy);
}

unique_ptr<Response> tello::Tello::back(int x) const {
    BackCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, *this, _strategy);
}

unique_ptr<Response> tello::Tello::clockwise_turn(int x) const {
    ClockwiseTurnCommand command{x};
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, *this, _strategy);
}

unique_ptr<Response> tello::Tello::counterclockwise_turn(int x) const {
    CounterclockwiseTurnCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, *this, _strategy);
}

unique_ptr<QueryResponse> tello::Tello::wifi() const {
    WifiCommand command;
    return Network::exec<QueryResponse, QueryResponse::error, QueryResponse::timeout, QueryResponse::of>(command, *this,
                                                                                                         _strategy);
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