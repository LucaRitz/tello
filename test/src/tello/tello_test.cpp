#include <gtest/gtest.h>
#include <tello/command_factory.hpp>
#include <tello/tello.hpp>
#include <tello/command.hpp>
#include <tello/response.hpp>
#include <chrono>
#include <thread>

#define TELLO_IP_ADDRESS (ULONG)0xC0A80A01 // 192.168.10.1

using tello::CommandFactory;
using tello::Command;
using tello::CommandType;
using tello::Tello;
using tello::Response;
using tello::Status;
using std::string;
using ComPtr = std::optional<std::unique_ptr<Command>>;

TEST(Tello, SimpleCaseBerger) {
    Tello tello(TELLO_IP_ADDRESS);

    ComPtr command = CommandFactory::build(CommandType::COMMAND);
    std::unique_ptr<Response> responseCommand = tello.exec(*(command->get()));
    ASSERT_EQ(Status::OK, responseCommand->status());

    ComPtr takeoff = CommandFactory::build(CommandType::TAKE_OFF);
    std::unique_ptr<Response> responseTakeoff = tello.exec(*(takeoff->get()));
    ASSERT_EQ(Status::OK, responseTakeoff->status());

    std::chrono::seconds duration(5);
    std::this_thread::sleep_for(duration);

    ComPtr land = CommandFactory::build(CommandType::LAND);
    std::unique_ptr<Response> responseLand = tello.exec(*(land->get()));
    ASSERT_EQ(Status::OK, responseLand->status());
}