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

unique_ptr<Command> tello::CommandFactory::buildCommand(const tello::CommandType& commandType, list<string>& arguments) {
    return tello::CommandFactory::MAPPING.find(commandType)->second(arguments);
}