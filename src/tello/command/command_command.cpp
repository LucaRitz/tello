#include "command_command.hpp"

using tello::Command;

tello::command::CommandCommand::CommandCommand(list<string>& arguments) : Command(arguments) {}

string tello::command::CommandCommand::build() const {
    return std::string("Command");
}

unique_ptr<Command> tello::command::CommandCommand::create(list<string>& arguments) {
    return std::make_unique<CommandCommand>(CommandCommand(arguments));
}
