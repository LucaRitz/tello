#pragma once

#include "command.hpp"
#include <tello/response/query_response.hpp>

using tello::QueryResponse;

namespace tello::command {

    class ReadWifiCommand : public Command {
    public:
        ReadWifiCommand();

        [[nodiscard]]
        string build() const override;
    };
}