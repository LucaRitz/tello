#include <gtest/gtest.h>
#include <command_factory.hpp>
#include <command.hpp>

using tello::CommandFactory;
using tello::Command;
using tello::CommandType;
using std::string;
using ComPtr = std::unique_ptr<Command>;

TEST(CommandFactory, CommandCommand) {
    std::vector<string> arguments;

    // Act
    ComPtr result = CommandFactory::build(CommandType::COMMAND, arguments);

    // Act
    ASSERT_TRUE(result->isValid());
    ASSERT_EQ(std::string("Command"), result->build());
}