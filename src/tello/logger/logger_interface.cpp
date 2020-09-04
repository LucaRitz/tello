#include <tello/logger/logger_interface.hpp>

#include "logger.hpp"

using tello::Logger;

tello::LoggerSettings::LoggerSettings(string commandFileLocation, string videoFileLocation,
                                      string statusFileLocation) : _commandFileLocation(std::move(commandFileLocation)),
                                                                   _videoFileLocation(std::move(videoFileLocation)),
                                                                   _statusFileLocation(std::move(statusFileLocation)) {

}

void tello::LoggerInterface::info(const LoggerType& loggerType, const string& message, const string& param1,
                                  const string& param2) {
    Logger::info(loggerType, message, param1, param2);
}

void tello::LoggerInterface::error(const LoggerType& loggerType, const string& message, const string& param1,
                                 const string& param2) {
    Logger::error(loggerType, message, param1, param2);
}

void tello::LoggerInterface::warn(const LoggerType& loggerType, const string& message, const string& param1,
                                  const string& param2) {
    Logger::warn(loggerType, message, param1, param2);
}

void tello::LoggerInterface::initialize(const LoggerSettings& settings) {
    Logger::initialize(settings);
}