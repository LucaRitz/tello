#pragma once

#include <vector>
#include <string>
#include "command_type.hpp"

using std::vector;
using std::string;

using t_forecast = unsigned int;

namespace tello {

    class StatusResponse;

    class Command {
    public:
        explicit Command(const CommandType& commandType, vector<string> arguments);
        Command(const Command& other) = default;
        Command(Command&& other) = default;
        virtual ~Command() = default;

        [[nodiscard]]
        virtual string validate() const;
        [[nodiscard]]
        virtual t_forecast forecast() const;
        [[nodiscard]]
        virtual t_forecast forecast(const StatusResponse& status) const;
        [[nodiscard]]
        virtual string build() const = 0;
        [[nodiscard]]
        inline const CommandType& type() const { return _type; }

    protected:
        const vector<string> _arguments;
        const CommandType _type;
    };
}