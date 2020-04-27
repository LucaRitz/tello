#define _WINSOCKAPI_

#include "gtest/gtest.h"
#include <tello/logger/logger.hpp>
#include <tello/connection/network.hpp>

using tello::LoggerSettings;
using tello::Logger;
using tello::Network;

int main(int argc, char* argv[]) {
    LoggerSettings settings {"./log/command_log.log", "./log/video_log.log", "./log/status_log.log"};
    Logger::initialize(settings);

    bool isConnected = Network::connect();
    assert(isConnected);

    ::testing::InitGoogleTest(&argc, argv);
    int returnVal = RUN_ALL_TESTS();

    Network::disconnect();
    return returnVal;
}