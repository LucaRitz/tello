#include "read_speed_command.hpp"

tello::command::ReadSpeedCommand::ReadSpeedCommand() :
	Command(CommandType::READ_SPEED) {}


string tello::command::ReadSpeedCommand::build() const {
    return std::string("speed?");
}