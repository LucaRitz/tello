#include <tello/logger/logger.hpp>
#include <spdlog/sinks/basic_file_sink.h>

std::shared_ptr<logger> tello::Logger::_commandLogger = spdlog::basic_logger_mt("command_logger_init", "./log/command_log_init.log");
std::shared_ptr<logger> tello::Logger::_statusLogger = spdlog::basic_logger_mt("status_logger_init", "./log/status_log_init.log");
std::shared_ptr<logger> tello::Logger::_videoLogger = spdlog::basic_logger_mt("video_logger_init", "./log/video_log_init.log");

tello::LoggerSettings::LoggerSettings(string commandFileLocation, string videoFileLocation,
                                      string statusFileLocation) : _commandFileLocation(std::move(commandFileLocation)),
                                                                   _videoFileLocation(std::move(videoFileLocation)),
                                                                   _statusFileLocation(std::move(statusFileLocation)) {}

void tello::Logger::initialize(const tello::LoggerSettings& settings) {
    _commandLogger = spdlog::basic_logger_mt("command_logger", settings._commandFileLocation);
    _videoLogger = spdlog::basic_logger_mt("video_logger", settings._videoFileLocation);
    _statusLogger = spdlog::basic_logger_mt("status_logger", settings._statusFileLocation);
    _commandLogger->flush_on(spdlog::level::err);
    _videoLogger->flush_on(spdlog::level::err);
    _statusLogger->flush_on(spdlog::level::err);
}

std::shared_ptr<logger> tello::Logger::get(const tello::LoggerType& loggerType) {

    if (_commandLogger == nullptr || _videoLogger == nullptr || _statusLogger == nullptr) {
        _commandLogger = spdlog::basic_logger_mt("command_logger_temp", "./log/command_log_temp.log");
        _videoLogger = spdlog::basic_logger_mt("video_logger_temp", "./log/video_log_temp.log");
        _statusLogger = spdlog::basic_logger_mt("status_logger_temp", "./log/status_log_temp.log");
    }

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