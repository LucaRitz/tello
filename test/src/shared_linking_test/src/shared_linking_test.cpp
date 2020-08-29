#include <gtest/gtest.h>
#include <tello/tello.hpp>
#include <tello/swarm.hpp>
#include <future>
#include <tello/response/query_response.hpp>
#include <tello/response/status_response.hpp>
#include <tello/logger/logger.hpp>

#define TELLO_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1
#define TELLO1_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1
#define TELLO2_IP_ADDRESS (ip_address)0xC0A80A02 // 192.168.10.2

using tello::Tello;
using tello::Response;
using tello::QueryResponse;
using tello::StatusResponse;
using tello::Status;
using std::string;
using std::promise;
using std::future;
using tello::QueryResponse;
using tello::Swarm;
using tello::Logger;
using tello::LoggerType;

TEST(SHARED_TEST, BasicFlightCommands) {
    Tello tello(TELLO_IP_ADDRESS);

    future<Response> command_future = tello.command();
    command_future.wait();
    ASSERT_NE(Status::FAIL, command_future.get().status());

    future<QueryResponse> wifi_future = tello.read_wifi();
    wifi_future.wait();
    ASSERT_NE(Status::FAIL, wifi_future.get().status());

    future<Response> takeoff_future = tello.takeoff();
    takeoff_future.wait();
    ASSERT_NE(Status::FAIL, takeoff_future.get().status());

    future<Response> up_future = tello.up(30);
    up_future.wait();
    ASSERT_NE(Status::FAIL, up_future.get().status());

    future<Response> down_future = tello.down(30);
    down_future.wait();
    ASSERT_NE(Status::FAIL, down_future.get().status());

    future<Response> left_future = tello.left(30);
    left_future.wait();
    ASSERT_NE(Status::FAIL, left_future.get().status());

    future<Response> right_future = tello.right(30);
    right_future.wait();
    ASSERT_NE(Status::FAIL, right_future.get().status());

    future<Response> forward_future = tello.forward(30);
    forward_future.wait();
    ASSERT_NE(Status::FAIL, forward_future.get().status());

    future<Response> back_future = tello.back(30);
    back_future.wait();
    ASSERT_NE(Status::FAIL, back_future.get().status());

    future<Response> clockwise_turn_future = tello.clockwise_turn(180);
    clockwise_turn_future.wait();
    ASSERT_NE(Status::FAIL, clockwise_turn_future.get().status());

    future<Response> counter_clockwise_turn_future = tello.counterclockwise_turn(180);
    counter_clockwise_turn_future.wait();
    ASSERT_NE(Status::FAIL, counter_clockwise_turn_future.get().status());

    future<Response> land_future = tello.land();
    land_future.wait();
    ASSERT_NE(Status::FAIL, land_future.get().status());
}

TEST(SHARED_TEST, SimpleCaseBerger_Swarm) {
    Swarm swarm;
    Tello tello1(TELLO1_IP_ADDRESS);
    Tello tello2(TELLO2_IP_ADDRESS);
    swarm << tello1 << tello2;

    unordered_map<ip_address, future<Response>> responseCommand = swarm.command();
    for(auto& entry : responseCommand) {
        entry.second.wait();
        ASSERT_NE(Status::FAIL, entry.second.get().status());
    }

    unordered_map<ip_address, future<Response>> responseTakeoff = swarm.takeoff();
    for(auto& entry : responseTakeoff) {
        entry.second.wait();
        ASSERT_NE(Status::FAIL, entry.second.get().status());
    }

    std::chrono::seconds duration(5);
    std::this_thread::sleep_for(duration);

    unordered_map<ip_address, future<Response>> responseLand = swarm.land();
    for(auto& entry : responseLand) {
        entry.second.wait();
        ASSERT_NE(Status::FAIL, entry.second.get().status());
    }
}


void handler(const VideoResponse&);
static int handlerCallCount = 0;

TEST(SHARED_TEST, SimpleCaseBergerVideo) {
    Tello tello(TELLO_IP_ADDRESS);
    tello.setVideoHandler(handler);

    future<Response> responseCommand = tello.command();
    responseCommand.wait();
    ASSERT_NE(Status::FAIL, responseCommand.get().status());

    future<Response> responseStreamOn = tello.streamon();
    responseStreamOn.wait();
    ASSERT_NE(Status::FAIL, responseStreamOn.get().status());

    std::chrono::seconds duration(15);
    std::this_thread::sleep_for(duration);

    future<Response> responseStreamOff = tello.streamoff();
    responseStreamOff.wait();
    ASSERT_NE(Status::FAIL, responseStreamOff.get().status());

    ASSERT_TRUE(handlerCallCount > 0);
}

void statusHandler(const StatusResponse&);
static int statusHandlerCallCount = 0;

TEST(SHARED_TEST, SimpleCaseBergerStatus) {
    Tello tello(TELLO_IP_ADDRESS);
    tello.setStatusHandler(statusHandler);

    future<Response> responseCommand = tello.command();
    responseCommand.wait();
    ASSERT_NE(Status::FAIL, responseCommand.get().status());

    std::chrono::seconds duration(15);
    std::this_thread::sleep_for(duration);

    ASSERT_TRUE(statusHandlerCallCount > 0);
}

void handler(const VideoResponse& response) {
    Logger::get(LoggerType::VIDEO)->info("VIDEO: {}",
                                         std::to_string(response.length()));
    handlerCallCount++;
}

void statusHandler(const StatusResponse& response) {
    Logger::get(LoggerType::STATUS)->info("STATUS: {}", std::to_string(response.get_agx()));
    statusHandlerCallCount++;
}