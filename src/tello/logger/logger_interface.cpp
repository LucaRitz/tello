#include <tello/logger/logger_interface.hpp>

#include "logger.hpp"
#include <spdlog/sinks/basic_file_sink.h>

using tello::Logger;

tello::LoggerSettings::LoggerSettings(string commandFileLocation, string videoFileLocation,
                                      string statusFileLocation) : _commandFileLocation(std::move(commandFileLocation)),
                                                                   _videoFileLocation(std::move(videoFileLocation)),
                                                                   _statusFileLocation(std::move(statusFileLocation)) {

}

void tello::LoggerInterface::info(const LoggerType& loggerType, const string& message, const string& params...) {
    Logger::info(loggerType, message, params);
}

void tello::LoggerInterface::error(const LoggerType& loggerType, const string& message, const string& params...) {
    Logger::error(loggerType, message, params);
}

void tello::LoggerInterface::warn(const LoggerType& loggerType, const string& message, const string& params...) {
    Logger::warn(loggerType, message, params);
}

void tello::LoggerInterface::initialize(const LoggerSettings& settings) {
    Logger::initialize(settings);
}