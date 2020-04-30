#include "takeoff_command.hpp"

using tello::Command;

tello::command::TakeoffCommand::TakeoffCommand(vector<string>& arguments) : Command(
        CommandType::TAKE_OFF,
        arguments) {}

t_forecast tello::command::TakeoffCommand::forecast() const {
    return 5;
}

string tello::command::TakeoffCommand::build() const {
    return std::string("takeoff");
}

unique_ptr<Command> tello::command::TakeoffCommand::create(vector<string>& arguments) {
    return std::make_unique<TakeoffCommand>(TakeoffCommand(arguments));
}