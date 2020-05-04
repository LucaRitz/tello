#include <gtest/gtest.h>
#include <tello/connection/udp_command_listener.hpp>
#include <tello/response.hpp>

using tello::UdpCommandListener;
using tello::Response;
using tello::ResponseMapping;
using tello::Status;

TEST(UdpCommandListener, cleanMapping_removeOlderThan15Seconds) {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    time_t nowInSec = std::chrono::duration_cast<std::chrono::seconds>(now).count();

    unordered_map<ip_address, vector<ResponseMapping>> mapping{};
    mapping[1] = vector<ResponseMapping>{ResponseMapping(std::make_shared<Response>(Status::UNKNOWN), nowInSec)};

    std::chrono::seconds awaitResponse(16);
    std::this_thread::sleep_for(awaitResponse);

    // Act
    vector<ResponseMapping> erased = UdpCommandListener::cleanMapping(mapping);

    // Assert
    ASSERT_TRUE(mapping.empty());
    ASSERT_FALSE(erased.empty());
}