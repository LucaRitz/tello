#include <gtest/gtest.h>
#include <tello/tello.hpp>
#include <future>

#define TELLO_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1

using tello::Command;
using tello::Tello;
using tello::Response;
using tello::QueryResponse;
using tello::Status;
using std::string;
using std::promise;
using std::future;

#include <iostream>

TEST(Tello, BasicFlightCommands) {
    Tello tello(TELLO_IP_ADDRESS);

    shared_ptr<Response> rp = tello.up(-30);
    promise<const Response&>& up_wrong_promise = rp->subscribe();
    future<const Response&> up_wrong_future = up_wrong_promise.get_future();
    up_wrong_future.wait();
    ASSERT_EQ(Status::FAIL, up_wrong_future.get().status());

    promise<const Response&>& command_promise = tello.command()->subscribe();
    future<const Response&> command_future = command_promise.get_future();
    command_future.wait();
    ASSERT_NE(Status::FAIL, command_future.get().status());

    promise<const Response&>& takeoff_promise = tello.takeoff()->subscribe();
    future<const Response&> takeoff_future = takeoff_promise.get_future();
    takeoff_future.wait();
    ASSERT_NE(Status::FAIL, takeoff_future.get().status());

    promise<const Response&>& up_promise = tello.up(30)->subscribe();
    future<const Response&> up_future = up_promise.get_future();
    up_future.wait();
    ASSERT_NE(Status::FAIL, up_future.get().status());

    promise<const Response&>& down_promise = tello.down(30)->subscribe();
    future<const Response&> down_future = down_promise.get_future();
    down_future.wait();
    ASSERT_NE(Status::FAIL, down_future.get().status());

    promise<const Response&>& left_promise = tello.left(30)->subscribe();
    future<const Response&> left_future = left_promise.get_future();
    left_future.wait();
    ASSERT_NE(Status::FAIL, left_future.get().status());

    promise<const Response&>& right_promise = tello.right(30)->subscribe();
    future<const Response&> right_future = right_promise.get_future();
    right_future.wait();
    ASSERT_NE(Status::FAIL, right_future.get().status());

    promise<const Response&>& forward_promise = tello.forward(30)->subscribe();
    future<const Response&> forward_future = forward_promise.get_future();
    forward_future.wait();
    ASSERT_NE(Status::FAIL, forward_future.get().status());

    promise<const Response&>& back_promise = tello.back(30)->subscribe();
    future<const Response&> back_future = back_promise.get_future();
    back_future.wait();
    ASSERT_NE(Status::FAIL, back_future.get().status());

    promise<const Response&>& clockwise_turn_promise = tello.clockwise_turn(180)->subscribe();
    future<const Response&> clockwise_turn_future = clockwise_turn_promise.get_future();
    clockwise_turn_future.wait();
    ASSERT_NE(Status::FAIL, clockwise_turn_future.get().status());

    promise<const Response&>& counter_clockwise_turn_promise = tello.counterclockwise_turn(180)->subscribe();
    future<const Response&> counter_clockwise_turn_future = counter_clockwise_turn_promise.get_future();
    counter_clockwise_turn_future.wait();
    ASSERT_NE(Status::FAIL, counter_clockwise_turn_future.get().status());

    promise<const Response&>& land_promise = tello.land()->subscribe();
    future<const Response&> land_future = land_promise.get_future();
    land_future.wait();
    ASSERT_NE(Status::FAIL, land_future.get().status());
}

/*
TEST(Tello, SpeedCommands) {
    Tello tello(TELLO_IP_ADDRESS);

    promise<const Response&>& command_promise = tello.command()->subscribe();
    future<const Response&> command_future = command_promise.get_future();
    command_future.wait();
    ASSERT_NE(Status::FAIL, command_future.get().status());

    //Read default speed
    int defaultVelocity = 50;
	
    promise<const QueryResponse&>& get_speed_promise = tello.read_speed()->subscribe();
    future<const QueryResponse&> get_speed_future = get_speed_promise.get_future();
    get_speed_future.wait();
    ASSERT_NE(Status::FAIL, get_speed_future.get().status());

	//Set speed to max 100
    int maxVelocity = 100;
    promise<const Response&>& set_speed_promise = tello.set_speed(maxVelocity)->subscribe();
    future<const Response&> set_speed_future = set_speed_promise.get_future();
    set_speed_future.wait();
    ASSERT_NE(Status::FAIL, set_speed_future.get().status());

	//Read that speed is max
    get_speed_promise = tello.read_speed()->subscribe();
    get_speed_future = get_speed_promise.get_future();
    get_speed_future.wait();
    ASSERT_NE(Status::FAIL, get_speed_future.get().status());

    //Reset speed to default speed
    set_speed_promise = tello.set_speed(defaultVelocity)->subscribe();
    set_speed_future = set_speed_promise.get_future();
    set_speed_future.wait();
    ASSERT_NE(Status::FAIL, set_speed_future.get().status());

	//Check if speed was rested to default speed
    get_speed_promise = tello.read_speed()->subscribe();
    get_speed_future = get_speed_promise.get_future();
    get_speed_future.wait();
    ASSERT_NE(Status::FAIL, get_speed_future.get().status());
	
}
*/