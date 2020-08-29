#pragma once
#include <memory>
#include "command.hpp"

using std::unique_ptr;

namespace tello::command {
    class SetSpeedCommand : public Command {
    public:
        explicit SetSpeedCommand(int velocity);

        [[nodiscard]]
        virtual string validate() const override;
        [[nodiscard]]
        string build() const override;

    private:
        const int _velocity;
    };
}