#pragma once

#include <unordered_map>
#include <memory>
#include "tello_interface.hpp"
#include "native/network_interface.hpp"
#include <future>

using std::unordered_map;
using std::shared_ptr;
using std::future;

namespace tello {

    class Tello;
    class Response;
    class QueryResponse;

    class Command;

    class Swarm
            : public TelloInterface<unordered_map<ip_address, future<Response>>, unordered_map<ip_address, future<QueryResponse>>> {
    public:
        Swarm() = default;

        void add(const Tello& tello);
        [[nodiscard]] const unordered_map<ip_address, const Tello*>& tellos() const;

        /////////////////////////////////////////////////////////////
        ///// COMMANDS //////////////////////////////////////////////
        /////////////////////////////////////////////////////////////

        [[nodiscard]] unordered_map<ip_address, future<Response>> command() const override;
        [[nodiscard]] unordered_map<ip_address, future<Response>> takeoff() const override;
        [[nodiscard]] unordered_map<ip_address, future<Response>> land() const override;

        [[nodiscard]] unordered_map<ip_address, future<Response>> streamon() const override;
        [[nodiscard]] unordered_map<ip_address, future<Response>> streamoff() const override;

        [[nodiscard]] unordered_map<ip_address, future<Response>> up(int x) const override;
        [[nodiscard]] unordered_map<ip_address, future<Response>> down(int x) const override;
        [[nodiscard]] unordered_map<ip_address, future<Response>> left(int x) const override;
        [[nodiscard]] unordered_map<ip_address, future<Response>> right(int x) const override;
        [[nodiscard]] unordered_map<ip_address, future<Response>> forward(int x) const override;
        [[nodiscard]] unordered_map<ip_address, future<Response>> back(int x) const override;

        [[nodiscard]] unordered_map<ip_address, future<Response>> clockwise_turn(int x) const override;
        [[nodiscard]] unordered_map<ip_address, future<Response>> counterclockwise_turn(int x) const override;

        [[nodiscard]] unordered_map<ip_address, future<Response>> flip(char flip_direction) const override;

    	[[nodiscard]] unordered_map<ip_address, future<Response>> stop() const override;
        [[nodiscard]] unordered_map<ip_address, future<Response>> emergency() const override;

        [[nodiscard]] unordered_map<ip_address, future<Response>> set_speed(int velocity) const override;
        [[nodiscard]] unordered_map<ip_address, future<Response>> rc_control(int x, int y, int z, int r) const override;

        [[nodiscard]] unordered_map<ip_address, future<QueryResponse>> read_speed() const override;
        [[nodiscard]] unordered_map<ip_address, future<QueryResponse>> read_wifi() const override;

        /////////////////////////////////////////////////////////////
        ///// END COMMANDS //////////////////////////////////////////
        /////////////////////////////////////////////////////////////

        Swarm& operator<<(const Tello&);

    private:
        unordered_map<ip_address, const Tello*> _tellos;
    };
}