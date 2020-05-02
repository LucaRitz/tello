#include "command_command.hpp"

using tello::Command;

tello::command::CommandCommand::CommandCommand() : Command(
        CommandType::COMMAND) {}

string tello::command::CommandCommand::build() const {
    return std::string("command");
}
