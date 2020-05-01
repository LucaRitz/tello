#include "streamon_command.hpp"

using tello::Command;

tello::command::StreamOnCommand::StreamOnCommand(vector<string>& arguments) : Command(
        CommandType::STREAM_ON,
        arguments) {}

string tello::command::StreamOnCommand::build() const {
    return std::string("streamon");
}

unique_ptr<Command> tello::command::StreamOnCommand::create(vector<string>& arguments) {
    return std::make_unique<StreamOnCommand>(arguments);
}
