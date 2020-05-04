#pragma once

#include <tello/command.hpp>
#include <tello/response/query_response.hpp>

using tello::QueryResponse;

namespace tello::command {

    class ReadSpeedCommand : public Command {
    public:
        ReadSpeedCommand();

        [[nodiscard]]
        string build() const override;
    };
}