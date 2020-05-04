#pragma once

#include <tello/command.hpp>
#include <tello/response/query_response.hpp>

using tello::QueryResponse;

namespace tello::command {

    class WifiCommand : public Command {
    public:
        WifiCommand();

        [[nodiscard]]
        string build() const override;
    };
}