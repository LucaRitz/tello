#include <tello/swarm.hpp>
#include "connection/network.hpp"
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
#include "command/flip_command.hpp"
#include "command/stop_command.hpp"
#include "command/emergency_command.hpp"
#include "command/set_speed_command.hpp"
#include "command/rc_control_command.hpp"
#include "command/read_speed_command.hpp"
#include "command/read_wifi_command.hpp"

using tello::Response;
using tello::QueryResponse;
using tello::Command;
using tello::Network;
using tello::Tello;

using namespace tello::command;

void tello::Swarm::add(const Tello &tello) {
    _tellos[tello.ip()] = &tello;
}

/////////////////////////////////////////////////////////////
///// COMMANDS //////////////////////////////////////////////
/////////////////////////////////////////////////////////////

unordered_map<ip_address, future<Response>> tello::Swarm::command() const {
    const CommandCommand command;
    return Network::exec<Response>(command, _tellos);
}

unordered_map<ip_address, future<Response>> tello::Swarm::takeoff() const {
    const TakeoffCommand command;
    return Network::exec<Response>(command, _tellos);
}

unordered_map<ip_address, future<Response>> tello::Swarm::land() const {
    const LandCommand command;
    return Network::exec<Response>(command, _tellos);
}


unordered_map<ip_address, future<Response>> tello::Swarm::streamon() const {
    const StreamOnCommand command;
    return Network::exec<Response>(command, _tellos);
}

unordered_map<ip_address, future<Response>> tello::Swarm::streamoff() const {
    const StreamOffCommand command;
    return Network::exec<Response>(command, _tellos);
}


unordered_map<ip_address, future<Response>> tello::Swarm::up(int x) const {
    const UpCommand command{ x };
    return Network::exec<Response>(command, _tellos);
}

unordered_map<ip_address, future<Response>> tello::Swarm::down(int x) const {
    const DownCommand command{ x };
    return Network::exec<Response>(command, _tellos);
}

unordered_map<ip_address, future<Response>> tello::Swarm::left(int x) const {
    const LeftCommand command{ x };
    return Network::exec<Response>(command, _tellos);
}

unordered_map<ip_address, future<Response>> tello::Swarm::right(int x) const {
    const RightCommand command{ x };
    return Network::exec<Response>(command, _tellos);
}

unordered_map<ip_address, future<Response>> tello::Swarm::forward(int x) const {
    const ForwardCommand command{ x };
    return Network::exec<Response>(command, _tellos);
}

unordered_map<ip_address, future<Response>> tello::Swarm::back(int x) const {
    const BackCommand command{ x };
    return Network::exec<Response>(command, _tellos);
}

unordered_map<ip_address, future<Response>> tello::Swarm::clockwise_turn(int x) const {
    const ClockwiseTurnCommand command{ x };
    return Network::exec<Response>(command, _tellos);
}

unordered_map<ip_address, future<Response>> tello::Swarm::counterclockwise_turn(int x) const {
    const CounterclockwiseTurnCommand command{ x };
    return Network::exec<Response>(command, _tellos);
}


unordered_map<ip_address, future<Response>> tello::Swarm::flip(char flip_direction) const {
    const FlipCommand command{ flip_direction };
    return Network::exec<Response>(command, _tellos);
}


unordered_map<ip_address, future<Response>> tello::Swarm::stop() const {
    const StopCommand command;
    return Network::exec<Response>(command, _tellos);
}

unordered_map<ip_address, future<Response>> tello::Swarm::emergency() const {
    const EmergencyCommand command;
    return Network::exec<Response>(command, _tellos);
}


unordered_map<ip_address, future<Response>> tello::Swarm::set_speed(int velocity) const {
    const SetSpeedCommand command { velocity };
    return Network::exec<Response>(command, _tellos);
}

unordered_map<ip_address, future<Response>> tello::Swarm::rc_control(int x, int y, int z, int r) const {
    const RCControlCommand command { x, y, z, r };
    return Network::exec<Response>(command, _tellos);
}

unordered_map<ip_address, future<QueryResponse>> tello::Swarm::read_speed() const {
    const ReadSpeedCommand command;
    return Network::exec<QueryResponse>(command, _tellos);
}

unordered_map<ip_address, future<QueryResponse>> tello::Swarm::read_wifi() const {
    const ReadWifiCommand command;
    return Network::exec<QueryResponse>(command, _tellos);
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