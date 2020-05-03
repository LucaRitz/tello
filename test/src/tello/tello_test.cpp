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

TEST(Tello, BasicFlightCommands) {
    Tello tello(TELLO_IP_ADDRESS);
    std::chrono::seconds duration(10);

    std::unique_ptr<Response> responseCommand = tello.command();
    ASSERT_NE(Status::FAIL, responseCommand->status());

    std::unique_ptr<Response> responseTakeoff = tello.takeoff();
    ASSERT_NE(Status::FAIL, responseTakeoff->status());
        
    std::this_thread::sleep_for(duration);

    std::unique_ptr<Response> responeUp = tello.up(30);
    ASSERT_NE(Status::FAIL, responeUp->status());

    std::this_thread::sleep_for(duration);
    
    std::unique_ptr<Response> responeDown = tello.down(30);
    ASSERT_NE(Status::FAIL, responeDown->status());

    std::this_thread::sleep_for(duration);

    std::unique_ptr<Response> responeLeft = tello.left(30);
    ASSERT_NE(Status::FAIL, responeLeft->status());

    std::this_thread::sleep_for(duration);

    std::unique_ptr<Response> responeRight = tello.right(30);
    ASSERT_NE(Status::FAIL, responeRight->status());

    std::this_thread::sleep_for(duration);

    std::unique_ptr<Response> responseForward = tello.forward(30);
    ASSERT_NE(Status::FAIL, responseForward->status());

    std::this_thread::sleep_for(duration);

    std::unique_ptr<Response> responseBack = tello.back(30);
    ASSERT_NE(Status::FAIL, responseBack->status());

    std::this_thread::sleep_for(duration);

    std::unique_ptr<Response> responseCWTurn = tello.clockwise_turn(180);
    ASSERT_NE(Status::FAIL, responseCWTurn->status());

    std::this_thread::sleep_for(duration);

    std::unique_ptr<Response> responseCCWTurn = tello.counterclockwise_turn(180);
    ASSERT_NE(Status::FAIL, responseCCWTurn->status());

    std::unique_ptr<Response> responseLand = tello.land();
    ASSERT_NE(Status::FAIL, responseLand->status());
}