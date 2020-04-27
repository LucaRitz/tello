#include "environment.hpp"
#include <tello/logger/logger.hpp>
#include <tello/connection/network.hpp>

using tello::LoggerSettings;
using tello::Logger;
using tello::Network;

tello::Environment::~Environment() {
    Network::disconnect();
}

void tello::Environment::Environment::SetUp() {
    LoggerSettings settings {"./log/command_log.log", "./log/video_log.log", "./log/status_log.log"};
    Logger::initialize(settings);

    const bool isConnected = Network::connect();
    assert(isConnected);
}

void tello::Environment::Environment::TearDown() {
    Network::disconnect();
}