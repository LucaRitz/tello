#include "clockwise_turn_command.hpp"
#include "validator_util.hpp"
#include <stdexcept>

using tello::Command;
using tello::validator::between;
using tello::validator::concat;

tello::command::ClockwiseTurnCommand::ClockwiseTurnCommand(int x) : Command(
    CommandType::CLOCKWISE_TURN), _x(x) {}

string tello::command::ClockwiseTurnCommand::validate() const {
    vector<string> errors;
    errors.push_back(between<int>(_x, 1, 360, "x"));

    return concat(errors);
}

string tello::command::ClockwiseTurnCommand::build() const {
    return std::string("cw ") + std::to_string(_x);
}