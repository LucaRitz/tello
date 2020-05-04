#include "back_command.hpp"
#include "validator_util.hpp"
#include <stdexcept>

using tello::Command;
using std::stoi;
using tello::validator::between;
using tello::validator::concat;

tello::command::BackCommand::BackCommand(int x) : Command(
    CommandType::BACK), _x(x) {}

string tello::command::BackCommand::validate() const {
    vector<string> errors;
    errors.push_back(between<int>(_x, 20, 500, "x"));

    return concat(errors);
}

string tello::command::BackCommand::build() const {
    return std::string("back ") + std::to_string(_x);
}