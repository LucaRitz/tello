#include "up_command.hpp"
#include "validator_util.hpp"
#include <stdexcept>

using tello::Command;
using std::stoi;
using tello::validator::between;
using tello::validator::concat;

tello::command::UpCommand::UpCommand(vector<string>& arguments) : Command(
        CommandType::UP,
        arguments) {}

string tello::command::UpCommand::validate() const {
    if (_arguments.empty()) {
        return string("Argument 'x' is missing");
    } else if (_arguments.size() > 1) {
        return string("Too much arguments given. Just 'x' expected");
    }

    vector<string> errors;
    errors.push_back(between<int, stoi>(_arguments[0], 20, 500, "x"));
    return concat(errors);
}

string tello::command::UpCommand::build() const {
    return std::string("up ") + _arguments[0];
}

unique_ptr<Command> tello::command::UpCommand::create(vector<string>& arguments) {
    return std::make_unique<UpCommand>(UpCommand(arguments));
}