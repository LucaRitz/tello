#include "streamon_command.hpp"

using tello::Command;

tello::command::StreamOnCommand::StreamOnCommand() : Command(
        CommandType::STREAM_ON) {}

string tello::command::StreamOnCommand::build() const {
    return std::string("streamon");
}