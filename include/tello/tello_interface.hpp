#pragma once

#include "connection/command_strategy.hpp"

using tello::CommandStrategy;

namespace tello {

    template<typename Response, typename QueryResponse>
    class TelloInterface {
    public:
        virtual Response command() const = 0;
        virtual Response takeoff() const = 0;
        virtual Response land() const = 0;
        virtual Response up(int x) const = 0;
        virtual Response streamon() const = 0;
        virtual Response streamoff() const = 0;
        virtual Response clockwise_turn(int x) const = 0;

        virtual QueryResponse wifi() const = 0;

        void setCommandStrategy(CommandStrategy strategy) {
            _strategy = strategy;
        }

    protected:
        explicit TelloInterface(CommandStrategy strategy) : _strategy(strategy) {}

        CommandStrategy _strategy;
    };
}