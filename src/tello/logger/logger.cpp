#include <tello/logger/logger.hpp>
#include <spdlog/sinks/basic_file_sink.h>

tello::LoggerSettings::LoggerSettings(const string& defaultFileLocation, const string& videoFileLocation,
                                      const string& statusFileLocation) : _defaultFileLocation(defaultFileLocation),
                                                                          _videoFileLocation(videoFileLocation),
                                                                          _statusFileLocation(statusFileLocation) {}

tello::Logger& tello::Logger::instance() {
    static Logger _instance;
    return _instance;
}

void tello::Logger::initialize(const tello::LoggerSettings& settings) {
    this->_defaultLogger = spdlog::basic_logger_mt("default_logger", settings._defaultFileLocation);
    this->_videoLogger = spdlog::basic_logger_mt("video_logger", settings._videoFileLocation);
    this->_statusLogger = spdlog::basic_logger_mt("status_logger", settings._statusFileLocation);
}

std::shared_ptr<logger> tello::Logger::get(const tello::LoggerType& loggerType) const {
    switch (loggerType) {
        case LoggerType::STATUS :
            return this->_statusLogger;
        case LoggerType::VIDEO :
            return this->_videoLogger;
        case LoggerType::DEFAULT:
        default:
            return this->_defaultLogger;
    }
}