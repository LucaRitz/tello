#include <tello/command.hpp>

tello::Command::Command(vector<string> arguments) : _arguments(std::move(arguments)) {}

bool tello::Command::isValid() const {
    return true;
}