#include "streamoff_command.hpp"

using tello::Command;

tello::command::StreamOffCommand::StreamOffCommand() : Command(
        CommandType::STREAM_OFF) {}

string tello::command::StreamOffCommand::build() const {
    return std::string("streamoff");
}
