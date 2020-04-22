#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <optional>

using std::vector;
using std::string;
using std::unordered_map;
using std::unique_ptr;
using std::optional;

namespace tello {

    class Command;

    using CommandBuildType = unique_ptr<Command> (*)(vector<string>&);

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
        static optional<unique_ptr<Command>> build(const CommandType& commandType);
        static optional<unique_ptr<Command>> build(const CommandType& commandType, vector<string>& arguments);

    private:
        static const unordered_map<const CommandType, CommandBuildType, EnumClassHash> MAPPING;
        static unordered_map<const CommandType, CommandBuildType, EnumClassHash> createMap();
    };
}