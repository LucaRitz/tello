#include <tello/command.hpp>

tello::Command::Command(const CommandType& commandType, vector<string> arguments) : _type(commandType),
                                                                                    _arguments(std::move(arguments)) {}

bool tello::Command::isValid() const {
    return true;
}