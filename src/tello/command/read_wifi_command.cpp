#include "read_wifi_command.hpp"

tello::command::ReadWifiCommand::ReadWifiCommand() :
	Command(CommandType::READ_WIFI) {}


string tello::command::ReadWifiCommand::build() const {
    return std::string("wifi?");
}