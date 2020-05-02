#pragma once

#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class LandCommand : public Command {
    public:
        LandCommand();

        [[nodiscard]]
        t_forecast forecast() const override;
        [[nodiscard]]
        t_forecast forecast(const StatusResponse& status) const override;
        [[nodiscard]]
        string build() const override;
    };
}