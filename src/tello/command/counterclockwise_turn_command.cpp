#include "counterclockwise_turn_command.hpp"
#include "validator_util.hpp"
#include <stdexcept>

using tello::Command;
using tello::validator::between;
using tello::validator::concat;

tello::command::CounterclockwiseTurnCommand::CounterclockwiseTurnCommand(int x) : Command(
    CommandType::COUNTERCLOCKWISE_TURN), _x(x) {}

string tello::command::CounterclockwiseTurnCommand::validate() const {
    vector<string> errors;
    errors.push_back(between<int>(_x, 1, 360, "x"));

    return concat(errors);
}

string tello::command::CounterclockwiseTurnCommand::build() const {
    return std::string("ccw ") + std::to_string(_x);
}