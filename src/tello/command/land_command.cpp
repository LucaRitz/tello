#include "land_command.hpp"

using tello::Command;

tello::command::LandCommand::LandCommand(const CommandType& commandType, vector<string>& arguments) : Command(
        commandType,
        arguments) {}

string tello::command::LandCommand::build() const {
    return std::string("land");
}

unique_ptr<Command> tello::command::LandCommand::create(const CommandType& commandType, vector<string>& arguments) {
    return std::make_unique<LandCommand>(LandCommand(commandType, arguments));
}