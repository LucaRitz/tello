#include <tello/command_type.hpp>

using tello::CommandType;
using tello::EnumClassHash;

unordered_map<const CommandType, string, EnumClassHash> tello::createNamesMap() {
    unordered_map<const CommandType, string, EnumClassHash> mapping;
    mapping[CommandType::COMMAND] = string("COMMAND");
    mapping[CommandType::TAKE_OFF] = string("TAKE_OFF");
    mapping[CommandType::LAND] = string("LAND");
    mapping[CommandType::UP] = string("UP");
    // TODO: Add Command-Mappings
    return mapping;
}