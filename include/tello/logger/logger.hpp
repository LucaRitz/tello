#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>

using std::string;
using spdlog::logger;

namespace tello {
    struct LoggerSettings {
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

    class Logger {
    public:
        [[nodiscard]]
        static std::shared_ptr<logger> get(const LoggerType& loggerType);
        static void initialize(const LoggerSettings& settings);

    private:
        Logger() = delete;

        static std::shared_ptr<logger> _commandLogger;
        static std::shared_ptr<logger> _videoLogger;
        static std::shared_ptr<logger> _statusLogger;
    };
}