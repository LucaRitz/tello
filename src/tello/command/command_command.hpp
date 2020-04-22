#pragma once

#include <memory>
#include <command.hpp>

using std::unique_ptr;

namespace tello::command {
    class CommandCommand : public Command {
    public:
        explicit CommandCommand(vector<string>& arguments);

        [[nodiscard]]
        string build() const override;

        static unique_ptr<Command> create(vector<string>& arguments);
    };
}