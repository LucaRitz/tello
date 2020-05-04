#include "emergency_command.hpp"

using tello::Command;

tello::command::EmergencyCommand::EmergencyCommand() :
	Command(CommandType::EMERGENCY) {}

string tello::command::EmergencyCommand::build() const {
    return std::string("emergency ");
}