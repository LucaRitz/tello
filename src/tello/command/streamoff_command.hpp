#pragma once

#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class StreamOffCommand : public Command {
    public:
        explicit StreamOffCommand(vector<string>& arguments);

        [[nodiscard]]
        string build() const override;

        static unique_ptr<Command> create(vector<string>& arguments);
    };
}