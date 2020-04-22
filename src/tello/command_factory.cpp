#include <command_factory.hpp>
#include <command.hpp>
#include "command/command_command.hpp"

using tello::Command;
using tello::CommandBuildType;
using tello::CommandType;
using tello::EnumClassHash;

const unordered_map<const CommandType, CommandBuildType, EnumClassHash> tello::CommandFactory::MAPPING = CommandFactory::createMap();

unordered_map<const CommandType, CommandBuildType, EnumClassHash> tello::CommandFactory::createMap() {
    unordered_map<const CommandType, CommandBuildType, EnumClassHash> mapping;
    mapping[CommandType::COMMAND] = command::CommandCommand::create;
    // TODO: Add Command-Mappings
    return mapping;
}

optional<unique_ptr<Command>> tello::CommandFactory::build(const CommandType& commandType) {
    vector<string> arguments;
    return CommandFactory::build(commandType, arguments);
}

optional<unique_ptr<Command>>
tello::CommandFactory::build(const tello::CommandType& commandType, vector<string>& arguments) {
    auto value = tello::CommandFactory::MAPPING.find(commandType);
    if (value != tello::CommandFactory::MAPPING.end()) {
        return std::make_optional<unique_ptr<Command>>(value->second(arguments));
    }
    return std::nullopt;
}