#pragma once

#include <list>
#include <string>

using std::list;
using std::string;

namespace tello {

    class Command {
    public:
        explicit Command(list<string> arguments);
        Command(const Command& other) = default;
        Command(Command&& other) = default;
        virtual ~Command() = default;

        [[nodiscard]]
        virtual bool isValid() const;
        [[nodiscard]]
        virtual string build() const = 0;

    protected:
        const list<string> _arguments;
    };
}