#pragma once

#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class TakeoffCommand : public Command {
    public:
        explicit TakeoffCommand(vector<string>& arguments);

        [[nodiscard]]
        t_forecast forecast() const override;
        [[nodiscard]]
        string build() const override;

        static unique_ptr<Command> create(vector<string>& arguments);
    };
}