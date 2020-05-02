#include "wifi_command.hpp"

tello::command::WifiCommand::WifiCommand() : Command(CommandType::QUERY_WIFI) {}


string tello::command::WifiCommand::build() const {
    return std::string("wifi?");
}