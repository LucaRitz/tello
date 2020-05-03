#include <tello/swarm.hpp>
#include <tello/connection/network.hpp>
#include <tello/tello.hpp>

#include "command/command_command.hpp"
#include "command/takeoff_command.hpp"
#include "command/land_command.hpp"
#include "command/up_command.hpp"
#include "command/streamon_command.hpp"
#include "command/streamoff_command.hpp"
#include "command/clockwise_turn_command.hpp"
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

unordered_map<ip_address, shared_ptr<Response>> tello::Swarm::command() const {
    CommandCommand command;
    return Network::exec<Response, Response::error, Response::empty>(command, _tellos, _strategy);
}

unordered_map<ip_address, shared_ptr<Response>> tello::Swarm::takeoff() const {
    TakeoffCommand command;
    return Network::exec<Response, Response::error, Response::empty>(command, _tellos, _strategy);
}

unordered_map<ip_address, shared_ptr<Response>> tello::Swarm::land() const {
    LandCommand command;
    return Network::exec<Response, Response::error, Response::empty>(command, _tellos, _strategy);
}

unordered_map<ip_address, shared_ptr<Response>> tello::Swarm::up(int x) const {
    UpCommand command{x};
    return Network::exec<Response, Response::error, Response::empty>(command, _tellos, _strategy);
}

unordered_map<ip_address, shared_ptr<Response>> tello::Swarm::streamon() const {
    StreamOnCommand command;
    return Network::exec<Response, Response::error, Response::empty>(command, _tellos, _strategy);
}

unordered_map<ip_address, shared_ptr<Response>> tello::Swarm::streamoff() const {
    StreamOffCommand command;
    return Network::exec<Response, Response::error, Response::empty>(command, _tellos, _strategy);
}

unordered_map<ip_address, shared_ptr<Response>> tello::Swarm::clockwise_turn(int x) const {
    ClockwiseTurnCommand command{x};
    return Network::exec<Response, Response::error, Response::empty>(command, _tellos, _strategy);
}

unordered_map<ip_address, shared_ptr<QueryResponse>> tello::Swarm::wifi() const {
    WifiCommand command;
    return Network::exec<QueryResponse, QueryResponse::error, QueryResponse::empty>(command,
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