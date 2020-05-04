#include <tello/command.hpp>

tello::Command::Command(const CommandType& commandType) : _type(commandType) {}

string tello::Command::validate() const {
    return string{};
}