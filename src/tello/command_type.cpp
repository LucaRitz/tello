#include <tello/command_type.hpp>

using tello::CommandType;
using tello::EnumClassHash;

unordered_map<const CommandType, string, EnumClassHash> tello::createNamesMap() {
    unordered_map<const CommandType, string, EnumClassHash> mapping;
    mapping[CommandType::COMMAND] = string("COMMAND");
    // TODO: Add Command-Mappings
    return mapping;
}