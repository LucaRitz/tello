#include <gtest/gtest.h>
#include <tello/command_factory.hpp>
#include <tello/command.hpp>
#include <tello/logger/logger.hpp>

using tello::LoggerSettings;
using tello::Logger;

using tello::CommandFactory;
using tello::Command;
using tello::CommandType;
using std::string;
using ComPtr = std::optional<std::unique_ptr<Command>>;

void initLogging();

TEST(CommandFactory, CommandCommand) {
    initLogging();

    // Act
    ComPtr result = CommandFactory::build(CommandType::COMMAND);

    // Act
    ASSERT_TRUE(result);
    ASSERT_TRUE(result.value()->isValid());
    ASSERT_EQ(std::string("Command"), result.value()->build());
}

void initLogging() {
    string logDir("./log/");
    const LoggerSettings settings{logDir + "default.txt", logDir + "video.txt", logDir + "status.txt"};
    Logger::instance().initialize(settings);
}