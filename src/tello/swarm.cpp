#include <tello/swarm.hpp>
#include <tello/connection/network.hpp>
#include <tello/tello.hpp>

#include "command/command_command.hpp"
#include "command/takeoff_command.hpp"
#include "command/land_command.hpp"
#include "command/streamon_command.hpp"
#include "command/streamoff_command.hpp"
#include "command/up_command.hpp"
#include "command/down_command.hpp"
#include "command/right_command.hpp"
#include "command/left_command.hpp"
#include "command/forward_command.hpp"
#include "command/back_command.hpp"
#include "command/clockwise_turn_command.hpp"
#include "command/counterclockwise_turn_command.hpp"
#include "command/wifi_command.hpp"

using tello::Response;
using tello::QueryResponse;
using tello::Command;
using tello::Network;
using tello::Tello;

using namespace tello::command;

tello::Swarm::Swarm() : TelloInterface(CommandStrategy::COMMAND_AND_WAIT) {}

void tello::Swarm::add(const Tello &tello) {
    _tellos[tello.ip()] = &tello;
}

/////////////////////////////////////////////////////////////
///// COMMANDS //////////////////////////////////////////////
/////////////////////////////////////////////////////////////

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::command() const {
    CommandCommand command;
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, _tellos, _strategy);
}

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::takeoff() const {
    TakeoffCommand command;
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, _tellos, _strategy);
}

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::land() const {
    LandCommand command;
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, _tellos, _strategy);
}

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::streamon() const {
    StreamOnCommand command;
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, _tellos, _strategy);
}

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::streamoff() const {
    StreamOffCommand command;
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, _tellos, _strategy);
}

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::up(int x) const {
    UpCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, _tellos, _strategy);
}

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::down(int x) const {
    DownCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, _tellos, _strategy);
}

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::left(int x) const {
    LeftCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, _tellos, _strategy);
}

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::right(int x) const {
    RightCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, _tellos, _strategy);
}

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::forward(int x) const {
    ForwardCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, _tellos, _strategy);
}

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::back(int x) const {
    BackCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, _tellos, _strategy);
}

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::clockwise_turn(int x) const {
    ClockwiseTurnCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, _tellos, _strategy);
}

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::counterclockwise_turn(int x) const {
    CounterclockwiseTurnCommand command{ x };
    return Network::exec<Response, Response::error, Response::timeout, Response::of>(command, _tellos, _strategy);
}

unordered_map<ip_address, unique_ptr<QueryResponse>> tello::Swarm::wifi() const {
    WifiCommand command;
    return Network::exec<QueryResponse, QueryResponse::error, QueryResponse::timeout, QueryResponse::of>(command,
                                                                                                         _tellos,
                                                                                                         _strategy);
}

/////////////////////////////////////////////////////////////
///// END COMMANDS //////////////////////////////////////////
/////////////////////////////////////////////////////////////

const unordered_map<ip_address, const tello::Tello*>& tello::Swarm::tellos() const {
    return _tellos;
}

tello::Swarm& tello::Swarm::operator<<(const Tello& tello) {
    add(tello);
    return *this;
}