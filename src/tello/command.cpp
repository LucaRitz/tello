#include <command.hpp>

tello::Command::Command(list<string> arguments) : _arguments(std::move(arguments)) {}

bool tello::Command::isValid() const {
    return true;
}