#include "flip_command.hpp"
#include "validator_util.hpp"
#include <stdexcept>

using tello::Command;
using tello::validator::isHorizontalDirection;
using tello::validator::concat;

tello::command::FlipCommand::FlipCommand(char flip_direction) :
	Command(CommandType::FLIP), _flip_direction(flip_direction) {}

string tello::command::FlipCommand::validate() const {
    vector<string> errors;
    errors.push_back(isHorizontalDirection(_flip_direction, "flip_direction"));

    return concat(errors);
}

string tello::command::FlipCommand::build() const {
    return std::string("flip ") + std::to_string(_flip_direction);
}