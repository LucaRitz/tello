#include <gtest/gtest.h>
#include <tello/command_factory.hpp>
#include <tello/tello.hpp>
#include <tello/command.hpp>
#include <chrono>
#include <thread>
#include <tello/logger/logger.hpp>

#define TELLO_IP_ADDRESS (ip_address)0xC0A80A01 // 192.168.10.1

using tello::CommandFactory;
using tello::Command;
using tello::CommandType;
using tello::Tello;
using tello::Response;
using tello::Status;
using tello::Logger;
using tello::LoggerType;
using std::string;
using ComPtr = std::optional<std::unique_ptr<Command>>;

void handler(const VideoResponse&);
static int handlerCallCount = 0;

TEST(Tello, SimpleCaseBergerVideo) {
    Tello tello(TELLO_IP_ADDRESS);
    tello.setVideoHandler(handler);

    ComPtr command = CommandFactory::build(CommandType::COMMAND);
    std::unique_ptr<Response> responseCommand = tello.exec(*(command.value()));
    ASSERT_NE(Status::FAIL, responseCommand->status());

    ComPtr streamon = CommandFactory::build(CommandType::STREAM_ON);
    std::unique_ptr<Response> responseStreamOn = tello.exec(*(streamon->get()));
    ASSERT_NE(Status::FAIL, responseStreamOn->status());

    std::chrono::seconds duration(10);
    std::this_thread::sleep_for(duration);

    ComPtr streamoff = CommandFactory::build(CommandType::STREAM_OFF);
    std::unique_ptr<Response> responseStreamOff = tello.exec(*(streamoff->get()));
    ASSERT_NE(Status::FAIL, responseStreamOff->status());

    ASSERT_TRUE(handlerCallCount > 0);
}

void handler(const VideoResponse& response) {
    if (!response.videoFrame().empty()) {
        Logger::get(LoggerType::VIDEO)->info("VIDEO: {}", response.videoFrame());
        handlerCallCount++;
    }
}