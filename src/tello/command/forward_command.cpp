#include "forward_command.hpp"
#include "validator_util.hpp"
#include <stdexcept>

using tello::Command;
using std::stoi;
using tello::validator::between;
using tello::validator::concat;

tello::command::ForwardCommand::ForwardCommand(int x) : Command(
    CommandType::FORWARD), _x(x) {}

string tello::command::ForwardCommand::validate() const {
    vector<string> errors;
    errors.push_back(between<int>(_x, 20, 500, "x"));

    return concat(errors);
}

string tello::command::ForwardCommand::build() const {
    return std::string("forward ") + std::to_string(_x);
}