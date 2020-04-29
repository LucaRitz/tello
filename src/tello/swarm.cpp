#include <tello/swarm.hpp>
#include <tello/connection/network.hpp>
#include <tello/tello.hpp>

using tello::Response;
using tello::Command;
using tello::Network;
using tello::Tello;

void tello::Swarm::add(const Tello &tello) {
    _tellos[tello.ip()] = &tello;
}

unordered_map<ip_address, unique_ptr<Response>> tello::Swarm::exec(const Command& command) {
    return Network::exec(command, _tellos);
}

const unordered_map<ip_address, const tello::Tello*>& tello::Swarm::tellos() const {
    return _tellos;
}

tello::Swarm& tello::Swarm::operator<<(const Tello& tello) {
    add(tello);
    return *this;
}