#pragma once
#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class RCControlCommand : public Command {
    public:
        explicit RCControlCommand(int x, int y, int z, int r);

        [[nodiscard]]
        string validate() const override;
        [[nodiscard]]
        string build() const override;
        [[nodiscard]]
        bool hasResponse() const override;

    private:
        const int _x;
        const int _y;
        const int _z;
        const int _r;
    };
}