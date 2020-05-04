#pragma once

#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class TakeoffCommand : public Command {
    public:
        TakeoffCommand();

        [[nodiscard]]
        string build() const override;
    };
}