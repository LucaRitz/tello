#pragma once

#include "command.hpp"

namespace tello::command {
    class CommandCommand : public Command {
    public:
        CommandCommand();

        [[nodiscard]]
        string build() const override;
    };
}