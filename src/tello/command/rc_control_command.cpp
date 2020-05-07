#include "rc_control_command.hpp"
#include "validator_util.hpp"
#include <stdexcept>

using tello::Command;
using std::stoi;
using tello::validator::between;
using tello::validator::concat;

tello::command::RCControlCommand::RCControlCommand(int x, int y, int z, int r) : Command(
        CommandType::RC_CONTROL), _x(x), _y(y), _z(z), _r(r) {}

string tello::command::RCControlCommand::validate() const {
    vector<string> errors;
    errors.push_back(between<int>(_x, -100, 100, "x"));
    errors.push_back(between<int>(_y, -100, 100, "y"));
    errors.push_back(between<int>(_z, -100, 100, "z"));
    errors.push_back(between<int>(_r, -100, 100, "r"));

    return concat(errors);
}

string tello::command::RCControlCommand::build() const {
    return std::string("rc ") + std::to_string(_x) + std::string(" ") + std::to_string(_y) + std::string(" ") + std::to_string(_z) + std::string(" ") + std::to_string(_r);
}