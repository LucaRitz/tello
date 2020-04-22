#pragma once

#include <vector>
#include <string>
#include "command_type.hpp"

using std::vector;
using std::string;

namespace tello {

    class Command {
    public:
        explicit Command(const CommandType& commandType, vector<string> arguments);
        Command(const Command& other) = default;
        Command(Command&& other) = default;
        virtual ~Command() = default;

        [[nodiscard]]
        virtual bool isValid() const;
        [[nodiscard]]
        virtual string build() const = 0;

        const CommandType& type() const;

    protected:
        const vector<string> _arguments;
        const CommandType _type;

    };
}