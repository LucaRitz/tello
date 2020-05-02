#pragma once

#include "connection/command_strategy.hpp"

using tello::CommandStrategy;

namespace tello {

    template<typename Response>
    class TelloInterface {
    public:
        virtual Response command() const = 0;
        virtual Response takeoff() const = 0;
        virtual Response land() const = 0;
        virtual Response up(int x) const = 0;
        virtual Response streamon() const = 0;
        virtual Response streamoff() const = 0;

        void setCommandStrategy(CommandStrategy strategy) {
            _strategy = strategy;
        }

    protected:
        TelloInterface(CommandStrategy strategy) : _strategy(strategy) {}

        CommandStrategy _strategy;
    };
}