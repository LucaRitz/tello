#pragma once

#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class LandCommand : public Command {
    public:
        LandCommand();

        [[nodiscard]]
        string build() const override;
    };
}