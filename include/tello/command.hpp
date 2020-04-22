#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace tello {

    class Command {
    public:
        explicit Command(vector<string> arguments);
        Command(const Command& other) = default;
        Command(Command&& other) = default;
        virtual ~Command() = default;

        [[nodiscard]]
        virtual bool isValid() const;
        [[nodiscard]]
        virtual string build() const = 0;

    protected:
        const vector<string> _arguments;
    };
}