#pragma once

#include <unordered_map>
#include <memory>
#include "tello_interface.hpp"
#include "native/network_interface.hpp"
#include "connection/command_strategy.hpp"

using std::unordered_map;
using std::unique_ptr;
using tello::CommandStrategy;

namespace tello {

    class Tello;
    class Response;
    class Command;

    class Swarm : public TelloInterface<unordered_map<ip_address, unique_ptr<Response>>> {
    public:
        Swarm();

        void add(const Tello& tello);
        [[nodiscard]] const unordered_map<ip_address, const Tello*>& tellos() const;

        /////////////////////////////////////////////////////////////
        ///// COMMANDS //////////////////////////////////////////////
        /////////////////////////////////////////////////////////////

        unordered_map<ip_address, unique_ptr<Response>> command() const;
        unordered_map<ip_address, unique_ptr<Response>> takeoff() const;
        unordered_map<ip_address, unique_ptr<Response>> land() const;
        unordered_map<ip_address, unique_ptr<Response>> up(int x) const;
        unordered_map<ip_address, unique_ptr<Response>> streamon() const;
        unordered_map<ip_address, unique_ptr<Response>> streamoff() const;

        /////////////////////////////////////////////////////////////
        ///// END COMMANDS //////////////////////////////////////////
        /////////////////////////////////////////////////////////////

        Swarm& operator<<(const Tello&);

    private:
        unordered_map<ip_address, const Tello*> _tellos;
    };
}