#include <gtest/gtest.h>
#include <command_factory.hpp>
#include <command.hpp>

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
    ASSERT_TRUE(result.value()->isValid());
    ASSERT_EQ(std::string("Command"), result.value()->build());
}