#pragma once

#include <memory>
#include "command.hpp"

using std::unique_ptr;

namespace tello::command {
    class EmergencyCommand : public Command {
    public:
        EmergencyCommand();

        [[nodiscard]]
        string build() const override;
    };
}