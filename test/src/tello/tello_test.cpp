#include <gtest/gtest.h>
#include <tello/tello.hpp>
#include <chrono>
#include <thread>

#define TELLO_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1

using tello::Command;
using tello::Tello;
using tello::Response;
using tello::Status;
using std::string;

bool successful = false;

TEST(Tello, BasicFlightCommands) {
    Tello tello(TELLO_IP_ADDRESS);
    std::chrono::seconds duration(10);

    tello.command()->subscribe([&tello](auto& response) {
        ASSERT_NE(Status::FAIL, response.status());
        tello.takeoff()->subscribe([&tello](auto& response) {
            ASSERT_NE(Status::FAIL, response.status());
            tello.up(30)->subscribe([&tello](auto& response) {
                ASSERT_NE(Status::FAIL, response.status());
                tello.down(30)->subscribe([&tello](auto& response) {
                    ASSERT_NE(Status::FAIL, response.status());
                    tello.left(30)->subscribe([&tello](auto& response) {
                        ASSERT_NE(Status::FAIL, response.status());
                        tello.right(30)->subscribe([&tello](auto& response) {
                            ASSERT_NE(Status::FAIL, response.status());
                            tello.forward(30)->subscribe([&tello](auto& response) {
                                ASSERT_NE(Status::FAIL, response.status());
                                tello.back(30)->subscribe([&tello](auto& response) {
                                    ASSERT_NE(Status::FAIL, response.status());
                                    tello.clockwise_turn(180)->subscribe([&tello](auto& response) {
                                        ASSERT_NE(Status::FAIL, response.status());
                                        tello.counterclockwise_turn(180)->subscribe([&tello](auto& response) {
                                            ASSERT_NE(Status::FAIL, response.status());
                                            tello.land()->subscribe([&tello](auto& response) {
                                                ASSERT_NE(Status::FAIL, response.status());
                                                successful = true;
                                            });
                                        });
                                    });
                                });
                            });
                        });
                    });
                });
            });
        });
    });

    while(!successful) {
        std::chrono::seconds duration(5);
        std::this_thread::sleep_for(duration);
    }

    ASSERT_TRUE(successful);
}