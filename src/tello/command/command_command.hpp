#pragma once

#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class CommandCommand : public Command {
    public:
        CommandCommand();

        [[nodiscard]]
        string build() const override;
    };
}