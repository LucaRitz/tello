#include "takeoff_command.hpp"

using tello::Command;

tello::command::TakeoffCommand::TakeoffCommand(const CommandType& commandType, vector<string>& arguments) : Command(
        commandType,
        arguments) {}

string tello::command::TakeoffCommand::build() const {
    return std::string("takeoff");
}

unique_ptr<Command> tello::command::TakeoffCommand::create(const CommandType& commandType, vector<string>& arguments) {
    return std::make_unique<TakeoffCommand>(TakeoffCommand(commandType, arguments));
}