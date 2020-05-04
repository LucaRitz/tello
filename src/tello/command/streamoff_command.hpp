#pragma once

#include <memory>
#include <tello/command.hpp>

using std::unique_ptr;

namespace tello::command {
    class StreamOffCommand : public Command {
    public:
        StreamOffCommand();

        [[nodiscard]]
        string build() const override;
    };
}