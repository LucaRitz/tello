#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <memory>

using std::list;
using std::string;
using std::unordered_map;
using std::unique_ptr;

namespace tello {

    class Command;

    using CommandBuildType = unique_ptr<Command> (*)(list<string>&);

    enum class CommandType {
        COMMAND
    };

    struct EnumClassHash
    {
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };

    class CommandFactory {

    public:
        static unique_ptr<Command> buildCommand(const CommandType& commandType, list<string>& arguments);

    private:
        static const unordered_map<const CommandType, CommandBuildType, EnumClassHash> MAPPING;
        static unordered_map<const CommandType, CommandBuildType, EnumClassHash> createMap();
    };
}