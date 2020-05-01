#include "streamoff_command.hpp"

using tello::Command;

tello::command::StreamOffCommand::StreamOffCommand(vector<string>& arguments) : Command(
        CommandType::STREAM_OFF,
        arguments) {}

string tello::command::StreamOffCommand::build() const {
    return std::string("streamoff");
}

unique_ptr<Command> tello::command::StreamOffCommand::create(vector<string>& arguments) {
    return std::make_unique<StreamOffCommand>(arguments);
}
