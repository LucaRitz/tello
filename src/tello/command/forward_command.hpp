#pragma once
#include <memory>
#include "command.hpp"

using std::unique_ptr;

namespace tello::command {
    class ForwardCommand : public Command {
    public:
        explicit ForwardCommand(int x);

        [[nodiscard]]
        virtual string validate() const override;
        [[nodiscard]]
        string build() const override;

    private:
        const int _x;
    };
}