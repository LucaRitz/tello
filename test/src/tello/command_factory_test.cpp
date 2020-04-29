#include <gtest/gtest.h>
#include <tello/command_factory.hpp>
#include <tello/command.hpp>
#include <random>

using tello::CommandFactory;
using tello::Command;
using tello::CommandType;
using std::string;
using ComPtr = std::optional<std::unique_ptr<Command>>;

static std::random_device rd;
static std::mt19937 mt(rd());

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

TEST(CommandFactory, UpCommand_noArgumentGiven_getErrorString) {
    // Act
    ComPtr result = CommandFactory::build(CommandType::UP);

    // Act
    ASSERT_TRUE(result);
    ASSERT_FALSE(result.value()->validate().empty());
}

TEST(CommandFactory, UpCommand_tooMuchArgumentsGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(0, 10);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    ComPtr result = CommandFactory::build(CommandType::UP, vector<string>{x_arg, x_arg});

    // Act
    ASSERT_TRUE(result);
    ASSERT_FALSE(result.value()->validate().empty());
}

TEST(CommandFactory, UpCommand_noIntegerValueGiven_getErrorString) {
    // Act
    ComPtr result = CommandFactory::build(CommandType::UP, vector<string>{"abab"});

    // Act
    ASSERT_TRUE(result);
    ASSERT_FALSE(result.value()->validate().empty());
}

TEST(CommandFactory, UpCommand_xValueLowerThanMinGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(0, 19);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    ComPtr result = CommandFactory::build(CommandType::UP, vector<string>{x_arg});

    // Act
    ASSERT_TRUE(result);
    ASSERT_FALSE(result.value()->validate().empty());
}

TEST(CommandFactory, UpCommand_xValueGreaterThanMaxGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(501);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    ComPtr result = CommandFactory::build(CommandType::UP, vector<string>{x_arg});

    // Act
    ASSERT_TRUE(result);
    ASSERT_FALSE(result.value()->validate().empty());
}

TEST(CommandFactory, UpCommand_xValueBetweenMinAndMaxGiven_buildExpectedCommand) {
    std::uniform_int_distribution<int> dist(20, 500);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    ComPtr result = CommandFactory::build(CommandType::UP, vector<string>{x_arg});

    // Act
    ASSERT_TRUE(result);
    ASSERT_TRUE(result.value()->validate().empty());
    ASSERT_EQ(std::string("up ") + x_arg, result.value()->build());
}