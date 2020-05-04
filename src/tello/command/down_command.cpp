#include "down_command.hpp"
#include "validator_util.hpp"
#include <stdexcept>

using tello::Command;
using std::stoi;
using tello::validator::between;
using tello::validator::concat;

tello::command::DownCommand::DownCommand(int x) : Command(
    CommandType::DOWN), _x(x) {}

string tello::command::DownCommand::validate() const {
    vector<string> errors;
    errors.push_back(between<int>(_x, 20, 500, "x"));

    return concat(errors);
}

string tello::command::DownCommand::build() const {
    return std::string("down ") + std::to_string(_x);
}