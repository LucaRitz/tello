#pragma once
#include <memory>
#include "command.hpp"

using std::unique_ptr;

namespace tello::command {
    class RightCommand : public Command {
    public:
        explicit RightCommand(int x);

        [[nodiscard]]
        virtual string validate() const override;
        [[nodiscard]]
        string build() const override;

    private:
        const int _x;
    };
}