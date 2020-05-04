#include "left_command.hpp"
#include "validator_util.hpp"
#include <stdexcept>

using tello::Command;
using std::stoi;
using tello::validator::between;
using tello::validator::concat;

tello::command::LeftCommand::LeftCommand(int x) : Command(
    CommandType::LEFT), _x(x) {}

string tello::command::LeftCommand::validate() const {
    vector<string> errors;
    errors.push_back(between<int>(_x, 20, 500, "x"));

    return concat(errors);
}

string tello::command::LeftCommand::build() const {
    return std::string("left ") + std::to_string(_x);
}