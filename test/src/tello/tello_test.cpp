#include <gtest/gtest.h>
#include <tello/tello.hpp>
#include <future>
#include <tello/response/query_response.hpp>

#define TELLO_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1

using tello::Command;
using tello::Tello;
using tello::Response;
using tello::Status;
using std::string;
using std::promise;
using std::future;
using tello::QueryResponse;

TEST(Tello, BasicFlightCommands) {
    Tello tello(TELLO_IP_ADDRESS);

    future<Response> command_future = tello.command();
    command_future.wait();
    ASSERT_EQ(Status::OK, command_future.get().status());

    future<QueryResponse> wifi_future = tello.read_wifi();
    wifi_future.wait();
    ASSERT_EQ(Status::OK, wifi_future.get().status());

    future<Response> takeoff_future = tello.takeoff();
    takeoff_future.wait();
    ASSERT_EQ(Status::OK, takeoff_future.get().status());

    future<Response> up_future = tello.up(30);
    up_future.wait();
    ASSERT_EQ(Status::OK, up_future.get().status());

    future<Response> down_future = tello.down(30);
    down_future.wait();
    ASSERT_EQ(Status::OK, down_future.get().status());

    future<Response> left_future = tello.left(30);
    left_future.wait();
    ASSERT_EQ(Status::OK, left_future.get().status());

    future<Response> right_future = tello.right(30);
    right_future.wait();
    ASSERT_EQ(Status::OK, right_future.get().status());

    future<Response> forward_future = tello.forward(30);
    forward_future.wait();
    ASSERT_EQ(Status::OK, forward_future.get().status());

    future<Response> back_future = tello.back(30);
    back_future.wait();
    ASSERT_EQ(Status::OK, back_future.get().status());

    future<Response> clockwise_turn_future = tello.clockwise_turn(180);
    clockwise_turn_future.wait();
    ASSERT_EQ(Status::OK, clockwise_turn_future.get().status());

    future<Response> counter_clockwise_turn_future = tello.counterclockwise_turn(180);
    counter_clockwise_turn_future.wait();
    ASSERT_EQ(Status::OK, counter_clockwise_turn_future.get().status());

    future<Response> land_future = tello.land();
    land_future.wait();
    ASSERT_EQ(Status::OK, land_future.get().status());
}