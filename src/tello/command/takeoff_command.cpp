#include "takeoff_command.hpp"

using tello::Command;

tello::command::TakeoffCommand::TakeoffCommand() : Command(
        CommandType::TAKE_OFF) {}

string tello::command::TakeoffCommand::build() const {
    return std::string("takeoff");
}