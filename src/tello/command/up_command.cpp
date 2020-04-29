#include "up_command.hpp"

#include <string>
#include <stdexcept>

using tello::Command;
using std::stoi;

tello::command::UpCommand::UpCommand(vector<string>& arguments) : Command(
        CommandType::UP,
        arguments) {}

string tello::command::UpCommand::validate() const {
    if (_arguments.empty()) {
        return string("Argument 'x' is missing");
    } else if (_arguments.size() > 1) {
        return string("Too much arguments given. Just 'x' expected");
    }

    try {
        int x = stoi(_arguments[0]);

        if (x < 20 || x > 500) {
            return string("Value of 'x' has to be between 20 and 500");
        }

        return string("");
    } catch(const std::invalid_argument& exc) {
        return string("Value of 'x' has to be between 20 and 500");
    } catch(const std::out_of_range& exc) {
        return string("Value of 'x' has to be between 20 and 500");
    }
}

string tello::command::UpCommand::build() const {
    return std::string("up ") + _arguments[0];
}

unique_ptr<Command> tello::command::UpCommand::create(vector<string>& arguments) {
    return std::make_unique<UpCommand>(UpCommand(arguments));
}