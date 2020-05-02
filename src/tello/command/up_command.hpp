#pragma once
#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class UpCommand : public Command {
    public:
        explicit UpCommand(int x);

        [[nodiscard]]
        virtual string validate() const override;
        [[nodiscard]]
        string build() const override;

    private:
        const int _x;
    };
}