#pragma once
#include <memory>
#include "command.hpp"

using std::unique_ptr;

namespace tello::command {
    class DownCommand : public Command {
    public:
        explicit DownCommand(int x);

        [[nodiscard]]
        virtual string validate() const override;
        [[nodiscard]]
        string build() const override;

    private:
        const int _x;
    };
}