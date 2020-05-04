#pragma once
#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class FlipCommand : public Command {
    public:
        explicit FlipCommand(char flip_direction);

        [[nodiscard]]
        virtual string validate() const override;
        [[nodiscard]]
        string build() const override;

    private:
        const char _flip_direction;
    };
}