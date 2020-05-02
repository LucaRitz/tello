#include "land_command.hpp"

using tello::Command;

tello::command::LandCommand::LandCommand() : Command(
        CommandType::LAND) {}

t_forecast tello::command::LandCommand::forecast() const {
    return 2000;
}

t_forecast tello::command::LandCommand::forecast(const StatusResponse& status) const {
    return forecast();
}

string tello::command::LandCommand::build() const {
    return std::string("land");
}