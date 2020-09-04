#pragma once

#include <string>
#include "../macro_definition.hpp"

using std::string;

namespace tello {

    class Logger;

    struct EXPORT LoggerSettings {
    public:
        LoggerSettings(string commandFileLocation, string videoFileLocation,
                       string statusFileLocation);

        const string _commandFileLocation;
        const string _videoFileLocation;
        const string _statusFileLocation;
    };

    enum class LoggerType {
        COMMAND,
        VIDEO,
        STATUS
    };

    class EXPORT LoggerInterface {
    public:
        LoggerInterface() = delete;

        static void info(const LoggerType& loggerType, const string& message, const string& param1 = "",
                         const string& param2 = "");
        static void error(const LoggerType& loggerType, const string& message, const string& param1 = "",
                          const string& param2 = "");
        static void warn(const LoggerType& loggerType, const string& message, const string& param1 = "",
                         const string& param2 = "");
        static void initialize(const LoggerSettings& settings);
    };
}