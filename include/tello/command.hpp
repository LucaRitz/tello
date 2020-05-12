#pragma once

#include <vector>
#include <string>
#include "command_type.hpp"

using std::vector;
using std::string;

namespace tello {

    class Command {
    public:
        explicit Command(const CommandType& commandType);
        Command(const Command& other) = default;
        Command(Command&& other) = default;
        virtual ~Command() = default;

        [[nodiscard]]
        virtual string validate() const;
        [[nodiscard]]
        virtual string build() const = 0;
        [[nodiscard]]
        virtual bool hasResponse() const;
        [[nodiscard]]
        inline const CommandType& type() const { return _type; }

    protected:
        const CommandType _type;
    };
}