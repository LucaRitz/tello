#include "up_command.hpp"
#include "validator_util.hpp"
#include <stdexcept>

using tello::Command;
using std::stoi;
using tello::validator::between;
using tello::validator::concat;

tello::command::UpCommand::UpCommand(int x) : Command(
        CommandType::UP), _x(x) {}

string tello::command::UpCommand::validate() const {
    vector<string> errors;
    errors.push_back(between<int>(_x, 20, 500, "x"));

    return concat(errors);
}

string tello::command::UpCommand::build() const {
    return std::string("up ") + std::to_string(_x);
}