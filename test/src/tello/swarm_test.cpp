#include <gtest/gtest.h>
#include <tello/command_factory.hpp>
#include <tello/tello.hpp>
#include <tello/swarm.hpp>
#include <tello/command.hpp>
#include <tello/response.hpp>
#include <chrono>
#include <thread>

#define TELLO1_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1
#define TELLO2_IP_ADDRESS (ip_address)0xC0A80A02 // 192.168.10.2

using tello::CommandFactory;
using tello::Command;
using tello::CommandType;
using tello::Tello;
using tello::Swarm;
using tello::Response;
using tello::Status;
using std::string;
using ComPtr = std::optional<std::unique_ptr<Command>>;

TEST(Swarm, SimpleCaseBerger_Swarm) {
    Swarm swarm;
    Tello tello1(TELLO1_IP_ADDRESS);
    Tello tello2(TELLO2_IP_ADDRESS);
    swarm << tello1 << tello2;

    ComPtr command = CommandFactory::build(CommandType::COMMAND);
    unordered_map<ip_address, std::unique_ptr<Response>> responseCommand = swarm.exec(*(command.value()));
    for(auto tello : swarm.tellos()) {
        ASSERT_NE(Status::FAIL, responseCommand.find(tello.first)->second->status());
    }

    ComPtr takeoff = CommandFactory::build(CommandType::TAKE_OFF);
    unordered_map<ip_address, std::unique_ptr<Response>> responseTakeoff = swarm.exec(*(takeoff->get()));
    for(auto tello : swarm.tellos()) {
        ASSERT_NE(Status::FAIL, responseTakeoff.find(tello.first)->second->status());
    }

    std::chrono::seconds duration(5);
    std::this_thread::sleep_for(duration);

    ComPtr land = CommandFactory::build(CommandType::LAND);
    unordered_map<ip_address, std::unique_ptr<Response>> responseLand = swarm.exec(*(land->get()));
    for(auto tello : swarm.tellos()) {
        ASSERT_NE(Status::FAIL, responseLand.find(tello.first)->second->status());
    }
}