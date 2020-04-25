#include "gtest/gtest.h"
#include <tello/logger/logger.hpp>

using tello::LoggerSettings;
using tello::Logger;

int main(int argc, char* argv[]) {
    LoggerSettings settings {"./log/command_log.log", "./log/video_log.log", "./log/status_log.log"};
    Logger::initialize(settings);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}