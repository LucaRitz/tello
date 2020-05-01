#pragma once

#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class StreamOnCommand : public Command {
    public:
        explicit StreamOnCommand(vector<string>& arguments);

        [[nodiscard]]
        string build() const override;

        static unique_ptr<Command> create(vector<string>& arguments);
    };
}