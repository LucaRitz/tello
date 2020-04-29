#include "land_command.hpp"

using tello::Command;

tello::command::LandCommand::LandCommand(vector<string>& arguments) : Command(
        CommandType::LAND,
        arguments) {}

string tello::command::LandCommand::build() const {
    return std::string("land");
}

unique_ptr<Command> tello::command::LandCommand::create(vector<string>& arguments) {
    return std::make_unique<LandCommand>(LandCommand(arguments));
}