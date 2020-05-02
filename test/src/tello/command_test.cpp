#include <gtest/gtest.h>
#include <tello/command/up_command.hpp>
#include <random>

using tello::Command;
using tello::CommandType;
using std::string;

static std::random_device rd;
static std::mt19937 mt(rd());

using namespace tello::command;

TEST(CommandFactory, UpCommand_xValueLowerThanMinGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(0, 19);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    UpCommand result = UpCommand(x);

    // Act
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, UpCommand_xValueGreaterThanMaxGiven_getErrorString) {
    std::uniform_int_distribution<int> dist(501);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    UpCommand result = UpCommand(x);

    // Act
    ASSERT_FALSE(result.validate().empty());
}

TEST(CommandFactory, UpCommand_xValueBetweenMinAndMaxGiven_buildExpectedCommand) {
    std::uniform_int_distribution<int> dist(20, 500);
    int x = dist(mt);
    string x_arg = std::to_string(x);

    // Act
    UpCommand result = UpCommand(x);

    // Act
    ASSERT_TRUE(result.validate().empty());
    ASSERT_EQ(std::string("up ") + x_arg, result.build());
}