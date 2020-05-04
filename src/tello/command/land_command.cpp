#include "land_command.hpp"

using tello::Command;

tello::command::LandCommand::LandCommand() : Command(
        CommandType::LAND) {}

string tello::command::LandCommand::build() const {
    return std::string("land");
}