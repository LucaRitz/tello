#pragma once
#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class BackCommand : public Command {
    public:
        explicit BackCommand(int x);

        [[nodiscard]]
        virtual string validate() const override;
        [[nodiscard]]
        string build() const override;

    private:
        const int _x;
    };
}