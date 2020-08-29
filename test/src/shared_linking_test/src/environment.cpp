#include "environment.hpp"
#include <tello/logger/logger.hpp>
#include <tello/connection/tello_network.hpp>

using tello::LoggerSettings;
using tello::Logger;
using tello::TelloNetwork;

tello::Environment::~Environment() {
}

void tello::Environment::Environment::SetUp() {
    LoggerSettings settings {"./log/command_log.log", "./log/video_log.log", "./log/status_log.log"};
    Logger::initialize(settings);

    const bool isConnected = TelloNetwork::connect();
    assert(isConnected);
}

void tello::Environment::Environment::TearDown() {
    TelloNetwork::disconnect();
}