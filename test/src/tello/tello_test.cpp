#include <gtest/gtest.h>
#include <tello/tello.hpp>
#include <tello/command.hpp>
#include <chrono>
#include <thread>

#define TELLO_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1

using tello::Command;
using tello::CommandType;
using tello::Tello;
using tello::Response;
using tello::Status;
using std::string;
using ComPtr = std::optional<std::unique_ptr<Command>>;

TEST(Tello, SimpleCaseBerger) {
    Tello tello(TELLO_IP_ADDRESS);

    std::unique_ptr<Response> responseCommand = tello.command();
    ASSERT_NE(Status::FAIL, responseCommand->status());

    std::unique_ptr<Response> responseTakeoff = tello.takeoff();
    ASSERT_NE(Status::FAIL, responseTakeoff->status());

    std::chrono::seconds duration(5);
    std::this_thread::sleep_for(duration);

    std::unique_ptr<Response> responeCWTurn = tello.clockwise_turn(180);
    ASSERT_NE(Status::FAIL, responeCWTurn->status());

    std::this_thread::sleep_for(duration);

    std::unique_ptr<Response> responseUp = tello.up(30);
    ASSERT_NE(Status::FAIL, responseUp->status());

    std::this_thread::sleep_for(duration);

    std::unique_ptr<Response> responseLand = tello.land();
    ASSERT_NE(Status::FAIL, responseLand->status());
}