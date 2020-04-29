#include <gtest/gtest.h>
#include <tello/command_factory.hpp>
#include <tello/command.hpp>

using tello::CommandFactory;
using tello::Command;
using tello::CommandType;
using std::string;
using ComPtr = std::optional<std::unique_ptr<Command>>;

TEST(CommandFactory, CommandCommand) {
    // Act
    ComPtr result = CommandFactory::build(CommandType::COMMAND);

    // Act
    ASSERT_TRUE(result);
    ASSERT_TRUE(result.value()->validate().empty());
    ASSERT_EQ(std::string("command"), result.value()->build());
}

TEST(CommandFactory, TakeoffCommand) {
    // Act
    ComPtr result = CommandFactory::build(CommandType::TAKE_OFF);

    // Act
    ASSERT_TRUE(result);
    ASSERT_TRUE(result.value()->validate().empty());
    ASSERT_EQ(std::string("takeoff"), result.value()->build());
}

TEST(CommandFactory, LandCommand) {
    // Act
    ComPtr result = CommandFactory::build(CommandType::LAND);

    // Act
    ASSERT_TRUE(result);
    ASSERT_TRUE(result.value()->validate().empty());
    ASSERT_EQ(std::string("land"), result.value()->build());
}