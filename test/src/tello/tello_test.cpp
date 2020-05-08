#include <gtest/gtest.h>
#include <tello/tello.hpp>
#include <future>
#include <tello/response/query_response.hpp>

#define TELLO_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1

using tello::Command;
using tello::Tello;
using tello::Response;
using tello::QueryResponse;
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


TEST(Tello, SpeedCommands) {
    Tello tello(TELLO_IP_ADDRESS);

    future<Response> command_future = tello.command();
    command_future.wait();
    ASSERT_NE(Status::FAIL, command_future.get().status());

    //Read default speed
    future<QueryResponse> get_speed_future = tello.read_speed();
    get_speed_future.wait();
    ASSERT_NE(Status::FAIL, get_speed_future.get().status());
    int defaultVelocity = get_speed_future.get().value();

	//Set speed to max 100
    int maxVelocity = 100;
    future<Response> set_speed_future= tello.set_speed(maxVelocity);
    set_speed_future.wait();
    ASSERT_NE(Status::FAIL, set_speed_future.get().status());

	//Read that speed is max
    get_speed_future = tello.read_speed();
    get_speed_future.wait();
    ASSERT_NE(Status::FAIL, get_speed_future.get().status());
    ASSERT_EQ(maxVelocity, get_speed_future.get().value());

    //Reset speed to default speed
    set_speed_future = tello.set_speed(defaultVelocity);
    set_speed_future.wait();
    ASSERT_NE(Status::FAIL, set_speed_future.get().status());

	//Check if speed was rested to default speed
    get_speed_future = tello.read_speed();
    get_speed_future.wait();
    ASSERT_NE(Status::FAIL, get_speed_future.get().status());
    ASSERT_EQ(defaultVelocity, get_speed_future.get().value());
}

TEST(Tello, RCControlCommandFlyCircle) {
    Tello tello(TELLO_IP_ADDRESS);

    future<Response> rc_control_future1 = tello.rc_control(10, -10, 0, 0);
    rc_control_future1.wait();
    ASSERT_EQ(Status::UNKNOWN, rc_control_future1.get().status());

    future<Response> rc_control_future2 = tello.rc_control(0, -10, 0, 0);
    rc_control_future2.wait();
    ASSERT_EQ(Status::UNKNOWN, rc_control_future2.get().status());

    future<Response> rc_control_future3 = tello.rc_control(-10, -10, 0, 0);
    rc_control_future3.wait();
    ASSERT_EQ(Status::UNKNOWN, rc_control_future3.get().status());

    future<Response> rc_control_future4 = tello.rc_control(-10, 0, 0, 0);
    rc_control_future4.wait();
    ASSERT_EQ(Status::UNKNOWN, rc_control_future4.get().status());

    future<Response> rc_control_future5 = tello.rc_control(-10, 10, 0, 0);
    rc_control_future5.wait();
    ASSERT_EQ(Status::UNKNOWN, rc_control_future5.get().status());

    future<Response> rc_control_future6 = tello.rc_control(0, 10, 0, 0);
    rc_control_future6.wait();
    ASSERT_EQ(Status::UNKNOWN, rc_control_future6.get().status());

    future<Response> rc_control_future7 = tello.rc_control(10, 10, 0, 0);
    rc_control_future7.wait();
    ASSERT_EQ(Status::UNKNOWN, rc_control_future7.get().status());

    future<Response> rc_control_future8 = tello.rc_control(10, 0, 0, 0);
    rc_control_future8.wait();
    ASSERT_EQ(Status::UNKNOWN, rc_control_future8.get().status());
}