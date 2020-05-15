#include <gtest/gtest.h>
#include <tello/tello.hpp>
#include <chrono>
#include <thread>
#include <tello/logger/logger.hpp>

#define TELLO_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1

using tello::Tello;
using tello::Response;
using tello::Status;
using tello::Logger;
using tello::LoggerType;
using std::string;

void handler(const VideoResponse&);
static int handlerCallCount = 0;

TEST(Tello, SimpleCaseBergerVideo) {
    Tello tello(TELLO_IP_ADDRESS);
    tello.setVideoHandler(handler);

    future<Response> responseCommand = tello.command();
    responseCommand.wait();
    ASSERT_NE(Status::FAIL, responseCommand.get().status());

    future<Response> responseStreamOn = tello.streamon();
    responseStreamOn.wait();
    ASSERT_NE(Status::FAIL, responseStreamOn.get().status());

    std::chrono::seconds duration(10);
    std::this_thread::sleep_for(duration);

    future<Response> responseStreamOff = tello.streamoff();
    responseStreamOff.wait();
    ASSERT_NE(Status::FAIL, responseStreamOff.get().status());

    ASSERT_TRUE(handlerCallCount > 0);
}

void handler(const VideoResponse& response) {
    Logger::get(LoggerType::VIDEO)->info("VIDEO: {}",
                                         std::string(reinterpret_cast<const char*>(response.videoFrame()),
                                                     response.length()));
    handlerCallCount++;
}