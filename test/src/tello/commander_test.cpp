#include <gtest/gtest.h>
#include <tello/command_factory.hpp>
#include <tello/commander.hpp>
#include <tello/command.hpp>
#include <tello/response.hpp>
#include <chrono>
#include <thread>
#include "logging.hpp"

using tello::CommandFactory;
using tello::Command;
using tello::CommandType;
using tello::Commander;
using tello::Response;
using tello::Status;
using std::string;
using ComPtr = std::optional<std::unique_ptr<Command>>;
using tello::test::initLogging;

TEST(Commander, SimpleCaseBerger) {
    initLogging();

    ComPtr command = CommandFactory::build(CommandType::COMMAND);
    std::unique_ptr<Response> responseCommand = Commander::instance().exec(*(command->get()));
    ASSERT_EQ(Status::OK, responseCommand->status());

    // ComPtr takeoff = CommandFactory::build(CommandType::TAKEOFF);
    // std::unique_ptr<Response> responseTakeoff = Commander::instance().exec(*(takeoff->get()));
    // ASSERT_EQ(Status::OK, responseTakeoff->status());

    std::chrono::seconds duration(5);
    std::this_thread::sleep_for(duration);

    // ComPtr land = CommandFactory::build(CommandType::LAND);
    // std::unique_ptr<Response> responseLand = Commander::instance().exec(*(land->get()));
    // ASSERT_EQ(Status::OK, responseLand->status());
}