#include <tello/command_factory.hpp>
#include <tello/command.hpp>
#include <tello/logger/logger.hpp>
#include "command/command_command.hpp"
#include "command/takeoff_command.hpp"
#include "command/land_command.hpp"
#include "command/up_command.hpp"

using tello::Command;
using tello::CommandBuildType;
using tello::CommandType;
using tello::EnumClassHash;
using tello::Logger;

const unordered_map<const CommandType, CommandBuildType, EnumClassHash> tello::CommandFactory::MAPPING = CommandFactory::createMap();

unordered_map<const CommandType, CommandBuildType, EnumClassHash> tello::CommandFactory::createMap() {
    unordered_map<const CommandType, CommandBuildType, EnumClassHash> mapping;
    mapping[CommandType::COMMAND] = command::CommandCommand::create;
    mapping[CommandType::TAKE_OFF] = command::TakeoffCommand::create;
    mapping[CommandType::LAND] = command::LandCommand::create;
    mapping[CommandType::UP] = command::UpCommand::create;
    // TODO: Add Command-Mappings
    return mapping;
}

optional<unique_ptr<Command>> tello::CommandFactory::build(const CommandType& commandType) {
    return CommandFactory::build(commandType, vector<string>{});
}

optional<unique_ptr<Command>>
tello::CommandFactory::build(const tello::CommandType& commandType, vector<string>& arguments) {
    return CommandFactory::build(commandType, std::move(arguments));
}


optional<unique_ptr<Command>>
tello::CommandFactory::build(const tello::CommandType& commandType, vector<string>&& arguments) {
    auto value = tello::CommandFactory::MAPPING.find(commandType);
    if (value != tello::CommandFactory::MAPPING.end()) {
        Logger::get(LoggerType::COMMAND)->info(
                std::string("Creating command [") + NAMES.find(commandType)->second + std::string("]"));
        return std::make_optional<unique_ptr<Command>>(value->second(arguments));
    }

    Logger::get(LoggerType::COMMAND)->critical(
            std::string("Command [") + NAMES.find(commandType)->second + std::string("] not found!"));
    return std::nullopt;
}