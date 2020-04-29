#pragma once
#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class UpCommand : public Command {
    public:
        explicit UpCommand(vector<string>& arguments);

        [[nodiscard]]
        virtual string validate() const override;
        [[nodiscard]]
        string build() const override;

        static unique_ptr<Command> create(vector<string>& arguments);
    };
}