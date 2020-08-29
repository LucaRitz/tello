#pragma once

#include <memory>
#include "command.hpp"

using std::unique_ptr;

namespace tello::command {
    class StreamOnCommand : public Command {
    public:
        StreamOnCommand();

        [[nodiscard]]
        string build() const override;
    };
}