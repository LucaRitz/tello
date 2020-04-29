#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <optional>
#include "command_type.hpp"

using std::vector;
using std::string;
using std::unordered_map;
using std::unique_ptr;
using std::optional;

namespace tello {

    class Command;

    using CommandBuildType = unique_ptr<Command> (*)(vector<string>&);

    class CommandFactory {

    public:
        CommandFactory() = delete;
        CommandFactory(const CommandFactory&) = delete;
        CommandFactory(const CommandFactory&&) = delete;

        static optional<unique_ptr<Command>> build(const CommandType& commandType);
        static optional<unique_ptr<Command>> build(const CommandType& commandType, vector<string>& arguments);
        static optional<unique_ptr<Command>> build(const CommandType& commandType, vector<string>&& arguments);

    private:
        static const unordered_map<const CommandType, CommandBuildType, EnumClassHash> MAPPING;
        static unordered_map<const CommandType, CommandBuildType, EnumClassHash> createMap();
    };
}