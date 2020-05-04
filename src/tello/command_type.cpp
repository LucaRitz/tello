#include <tello/command_type.hpp>

using tello::CommandType;
using tello::EnumClassHash;

unordered_map<const CommandType, string, EnumClassHash> tello::createNamesMap() {
    unordered_map<const CommandType, string, EnumClassHash> mapping;
    mapping[CommandType::COMMAND] = string("COMMAND");
    mapping[CommandType::TAKE_OFF] = string("TAKE_OFF");
    mapping[CommandType::LAND] = string("LAND");
	
    mapping[CommandType::STREAM_ON] = string("STREAM_ON");
    mapping[CommandType::STREAM_OFF] = string("STREAM_OFF");
	
    mapping[CommandType::UP] = string("UP");
    mapping[CommandType::DOWN] = string("DOWN");
    mapping[CommandType::LEFT] = string("LEFT");
    mapping[CommandType::RIGHT] = string("RIGHT");
    mapping[CommandType::FORWARD] = string("FORWARD");
    mapping[CommandType::BACK] = string("BACK");
	
    mapping[CommandType::CLOCKWISE_TURN] = string("CLOCKWISE_TURN");
    mapping[CommandType::COUNTERCLOCKWISE_TURN] = string("COUNTERCLOCKWISE_TURN");

    mapping[CommandType::FLIP] = string("FLIP");

    mapping[CommandType::STOP] = string("STOP");
    mapping[CommandType::EMERGENCY] = string("EMERGENCY");

    mapping[CommandType::SET_SPEED] = string("SET_SPEED");

	mapping[CommandType::READ_SPEED] = string("READ_SPEED");
	mapping[CommandType::READ_WIFI] = string("READ_WIFI");
    // TODO: Add Command-Mappings
    return mapping;
}