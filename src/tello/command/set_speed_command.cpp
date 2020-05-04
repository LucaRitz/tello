#include "set_speed_command.hpp"
#include "validator_util.hpp"
#include <stdexcept>

using tello::Command;
using std::stoi;
using tello::validator::between;
using tello::validator::concat;

tello::command::SetSpeedCommand::SetSpeedCommand(int velocity) :
	Command(CommandType::SET_SPEED), _velocity(velocity) {}

string tello::command::SetSpeedCommand::validate() const {
    vector<string> errors;
    errors.push_back(between<int>(_velocity, 10, 100, "velocity"));

    return concat(errors);
}

string tello::command::SetSpeedCommand::build() const {
    return std::string("speed ") + std::to_string(_velocity);
}