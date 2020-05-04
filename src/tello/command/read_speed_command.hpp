#pragma once

#include <tello/command.hpp>

namespace tello::command {

    class ReadSpeedCommand : public Command {
    public:
        ReadSpeedCommand();

        [[nodiscard]]
        string build() const override;
    };
}