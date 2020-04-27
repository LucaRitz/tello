#include "command_command.hpp"

using tello::Command;

tello::command::CommandCommand::CommandCommand(const CommandType& commandType, vector<string>& arguments) : Command(
        commandType,
        arguments) {}

string tello::command::CommandCommand::build() const {
    return std::string("command");
}

unique_ptr<Command> tello::command::CommandCommand::create(const CommandType& commandType, vector<string>& arguments) {
    return std::make_unique<CommandCommand>(CommandCommand(commandType, arguments));
}
