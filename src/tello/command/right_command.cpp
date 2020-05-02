#include "right_command.hpp"
#include "validator_util.hpp"
#include <stdexcept>

using tello::Command;
using std::stoi;
using tello::validator::between;
using tello::validator::concat;

tello::command::RightCommand::RightCommand(int x) : Command(
    CommandType::RIGHT), _x(x) {}

string tello::command::RightCommand::validate() const {
    vector<string> errors;
    errors.push_back(between<int>(_x, 20, 500, "x"));

    return concat(errors);
}

string tello::command::RightCommand::build() const {
    return std::string("right ") + std::to_string(_x);
}