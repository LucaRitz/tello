#pragma once

#include <string>
#include <memory>
#include <tello/logger/logger_interface.hpp>

using std::string;

namespace spdlog {
    class logger;
}

using spdlog::logger;

namespace tello {

    enum class LoggerType;

    class Logger {
    public:
        Logger() = delete;


        static void info(const LoggerType& loggerType, const string& message, const string& param1 = "",
                         const string& param2 = "");
        static void error(const LoggerType& loggerType, const string& message, const string& param1 = "",
                         const string& param2 = "");
        static void warn(const LoggerType& loggerType, const string& message, const string& param1 = "",
                const string& param2 = "");
        static void initialize(const LoggerSettings& settings);

    private:
        static std::shared_ptr<logger> _commandLogger;
        static std::shared_ptr<logger> _videoLogger;
        static std::shared_ptr<logger> _statusLogger;

        static std::shared_ptr<logger> logger(const LoggerType& loggerType);
    };
}