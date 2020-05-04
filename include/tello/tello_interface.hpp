#pragma once

namespace tello {

    template<typename Response, typename QueryResponse>
    class TelloInterface {
    public:
        virtual Response command() const = 0;
        virtual Response takeoff() const = 0;
        virtual Response land() const = 0;
        virtual Response streamon() const = 0;
        virtual Response streamoff() const = 0;
        virtual Response up(int x) const = 0;
        virtual Response down(int x) const = 0;
        virtual Response left(int x) const = 0;
        virtual Response right(int x) const = 0;
        virtual Response forward(int x) const = 0;
        virtual Response back(int x) const = 0;
        virtual Response clockwise_turn(int x) const = 0;
        virtual Response counterclockwise_turn(int x) const = 0;

        virtual QueryResponse wifi() const = 0;

    protected:
        TelloInterface() = default;
    };
}