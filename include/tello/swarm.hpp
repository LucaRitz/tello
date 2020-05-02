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
    class QueryResponse;

    class Command;

    class Swarm
            : public TelloInterface<unordered_map<ip_address, unique_ptr<Response>>, unordered_map<ip_address, unique_ptr<QueryResponse>>> {
    public:
        Swarm();

        void add(const Tello& tello);
        [[nodiscard]] const unordered_map<ip_address, const Tello*>& tellos() const;

        /////////////////////////////////////////////////////////////
        ///// COMMANDS //////////////////////////////////////////////
        /////////////////////////////////////////////////////////////

        [[nodiscard]] unordered_map<ip_address, unique_ptr<Response>> command() const override;
        [[nodiscard]] unordered_map<ip_address, unique_ptr<Response>> takeoff() const override;
        [[nodiscard]] unordered_map<ip_address, unique_ptr<Response>> land() const override;
        [[nodiscard]] unordered_map<ip_address, unique_ptr<Response>> up(int x) const override;
        [[nodiscard]] unordered_map<ip_address, unique_ptr<Response>> streamon() const override;
        [[nodiscard]] unordered_map<ip_address, unique_ptr<Response>> streamoff() const override;
        [[nodiscard]] unordered_map<ip_address, unique_ptr<Response>> clockwise_turn(int x) const override;

        [[nodiscard]] unordered_map<ip_address, unique_ptr<QueryResponse>> wifi() const override;

        /////////////////////////////////////////////////////////////
        ///// END COMMANDS //////////////////////////////////////////
        /////////////////////////////////////////////////////////////

        Swarm& operator<<(const Tello&);

    private:
        unordered_map<ip_address, const Tello*> _tellos;
    };
}