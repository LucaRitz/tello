#include "gtest/gtest.h"
#include <string>
#include <tello/logger/logger.hpp>

using std::string;
using tello::LoggerSettings;
using tello::Logger;


int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);

    string logDir("./log/");
    const LoggerSettings settings{logDir + "default.txt", logDir + "video.txt", logDir + "status.txt"};
    Logger::instance().initialize(settings);

    return RUN_ALL_TESTS();
}