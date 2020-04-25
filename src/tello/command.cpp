#include <tello/command.hpp>

tello::Command::Command(const CommandType& commandType, vector<string> arguments) : _type(commandType),
                                                                                    _arguments(std::move(arguments)) {}

string tello::Command::validate() const {
    return string{};
}