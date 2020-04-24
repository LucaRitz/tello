#pragma once

#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class TakeoffCommand : public Command {
    public:
        explicit TakeoffCommand(const CommandType& commandType, vector<string>& arguments);

        [[nodiscard]]
        string build() const override;

        static unique_ptr<Command> create(const CommandType& commandType, vector<string>& arguments);
    };
}