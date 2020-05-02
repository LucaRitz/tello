#include "takeoff_command.hpp"

using tello::Command;

tello::command::TakeoffCommand::TakeoffCommand() : Command(
        CommandType::TAKE_OFF) {}

t_forecast tello::command::TakeoffCommand::forecast() const {
    return 5000;
}

string tello::command::TakeoffCommand::build() const {
    return std::string("takeoff");
}