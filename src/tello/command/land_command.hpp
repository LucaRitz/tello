#pragma once

#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class LandCommand : public Command {
    public:
        explicit LandCommand(vector<string>& arguments);

        [[nodiscard]]
        t_forecast forecast() const override;
        [[nodiscard]]
        t_forecast forecast(const StatusResponse& status) const override;
        [[nodiscard]]
        string build() const override;

        static unique_ptr<Command> create(vector<string>& arguments);
    };
}