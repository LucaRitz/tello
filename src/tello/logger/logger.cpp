#include "logger.hpp"
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <tello/logger/logger_interface.hpp>

std::shared_ptr<logger> tello::Logger::_commandLogger = spdlog::basic_logger_mt("command_logger_init", "./log/command_log_init.log");
std::shared_ptr<logger> tello::Logger::_statusLogger = spdlog::basic_logger_mt("status_logger_init", "./log/status_log_init.log");
std::shared_ptr<logger> tello::Logger::_videoLogger = spdlog::basic_logger_mt("video_logger_init", "./log/video_log_init.log");

void tello::Logger::initialize(const tello::LoggerSettings& settings) {
    _commandLogger = spdlog::basic_logger_mt("command_logger", settings._commandFileLocation);
    _videoLogger = spdlog::basic_logger_mt("video_logger", settings._videoFileLocation);
    _statusLogger = spdlog::basic_logger_mt("status_logger", settings._statusFileLocation);
    _commandLogger->flush_on(spdlog::level::err);
    _videoLogger->flush_on(spdlog::level::err);
    _statusLogger->flush_on(spdlog::level::err);
}

void
tello::Logger::info(const LoggerType& loggerType, const string& message, const string& param1, const string& param2) {
    auto logger = tello::Logger::logger(loggerType);
    logger->info(message, param1, param2);
}

void
tello::Logger::error(const LoggerType& loggerType, const string& message, const string& param1, const string& param2) {
    auto logger = tello::Logger::logger(loggerType);
    logger->error(message, param1, param2);
}

void
tello::Logger::warn(const LoggerType& loggerType, const string& message, const string& param1, const string& param2) {
    auto logger = tello::Logger::logger(loggerType);
    logger->warn(message, param1, param2);
    logger->flush();
}

std::shared_ptr<logger> tello::Logger::logger(const LoggerType& loggerType) {
    switch (loggerType) {
        case LoggerType::STATUS :
            return _statusLogger;
        case LoggerType::VIDEO :
            return _videoLogger;
        case LoggerType::COMMAND:
        default:
            return _commandLogger;
    }
}