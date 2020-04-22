#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>

using std::string;
using spdlog::logger;

namespace tello {
    struct LoggerSettings {
    public:
        LoggerSettings(const string& defaultFileLocation, const string& videoFileLocation,
                       const string& statusFileLocation);

        const string _defaultFileLocation;
        const string _videoFileLocation;
        const string _statusFileLocation;
    };

    enum class LoggerType {
        DEFAULT,
        VIDEO,
        STATUS
    };

    class Logger {
    public:
        static tello::Logger& instance();

        [[nodiscard]]
        std::shared_ptr<logger> get(const LoggerType& loggerType) const;
        void initialize(const LoggerSettings& settings);
    private:
        Logger() = default;

        std::shared_ptr<logger> _defaultLogger;
        std::shared_ptr<logger> _videoLogger;
        std::shared_ptr<logger> _statusLogger;
    };
}