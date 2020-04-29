#include "command_command.hpp"

using tello::Command;

tello::command::CommandCommand::CommandCommand(vector<string>& arguments) : Command(
        CommandType::COMMAND,
        arguments) {}

string tello::command::CommandCommand::build() const {
    return std::string("command");
}

unique_ptr<Command> tello::command::CommandCommand::create(vector<string>& arguments) {
    return std::make_unique<CommandCommand>(CommandCommand(arguments));
}
