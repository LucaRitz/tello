#include "stop_command.hpp"

using tello::Command;

tello::command::StopCommand::StopCommand() :
	Command(CommandType::STOP) {}

string tello::command::StopCommand::build() const {
	return std::string("stop ");
}