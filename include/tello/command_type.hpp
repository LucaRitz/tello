#pragma once

#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;

namespace tello {

    struct EnumClassHash
    {
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };

    enum class CommandType {
        COMMAND,

        TAKE_OFF,
        LAND,

        STREAM_ON,
        STREAM_OFF,

        UP,
        DOWN,
        LEFT,
        RIGHT,
        FORWARD,
        BACK,

        CLOCKWISE_TURN,
        COUNTERCLOCKWISE_TURN,

        FLIP,

        EMERGENCY,
        STOP,

        SET_SPEED,
        RC_CONTROL,

        READ_SPEED,
        READ_WIFI
    };

    unordered_map<const CommandType, string, EnumClassHash> createNamesMap();
    static const unordered_map<const CommandType, string, EnumClassHash> NAMES = createNamesMap();
}