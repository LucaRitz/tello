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

    std::shared_ptr<Response> responseCommand = tello.command();
    ASSERT_NE(Status::FAIL, responseCommand->status());

    std::shared_ptr<Response> responseStreamOn = tello.streamon();
    ASSERT_NE(Status::FAIL, responseStreamOn->status());

    std::chrono::seconds duration(10);
    std::this_thread::sleep_for(duration);

    std::shared_ptr<Response> responseStreamOff = tello.streamoff();
    ASSERT_NE(Status::FAIL, responseStreamOff->status());

    ASSERT_TRUE(handlerCallCount > 0);
}

void handler(const VideoResponse& response) {
    if (!response.videoFrame().empty()) {
        Logger::get(LoggerType::VIDEO)->info("VIDEO: {}", response.videoFrame());
        handlerCallCount++;
    }
}