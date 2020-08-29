#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include "../macro_definition.hpp"

using std::string;
using spdlog::logger;

namespace tello {
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

    class EXPORT Logger {
    public:
        Logger() = delete;
        [[nodiscard]]
        static std::shared_ptr<logger> get(const LoggerType& loggerType);
        static void initialize(const LoggerSettings& settings);

    private:
        static std::shared_ptr<logger> _commandLogger;
        static std::shared_ptr<logger> _videoLogger;
        static std::shared_ptr<logger> _statusLogger;
    };
}