#include <gtest/gtest.h>
#include <tello/tello.hpp>
#include <tello/swarm.hpp>
#include <tello/command.hpp>
#include <chrono>
#include <thread>

#define TELLO1_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1
#define TELLO2_IP_ADDRESS (ip_address)0xC0A80A02 // 192.168.10.2

using tello::Command;
using tello::CommandType;
using tello::Tello;
using tello::Swarm;
using tello::Response;
using tello::Status;
using std::string;

TEST(Swarm, SimpleCaseBerger_Swarm) {
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

    std::chrono::seconds awaitResponse(16);
    std::this_thread::sleep_for(awaitResponse);
}