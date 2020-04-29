#pragma once

#include <unordered_map>
#include <memory>
#include <tello/native/network_interface.hpp>

using std::unordered_map;
using std::unique_ptr;

namespace tello {

    class Tello;
    class Response;
    class Command;

    class Swarm {
    public:
        Swarm() = default;

        void add(const Tello& tello);
        unordered_map<ip_address, unique_ptr<Response>> exec(const Command& command);
        [[nodiscard]] const unordered_map<ip_address, const Tello*>& tellos() const;

        Swarm& operator<<(const Tello&);

    private:
        unordered_map<ip_address, const Tello*> _tellos;
    };
}