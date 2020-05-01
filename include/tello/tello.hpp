#pragma once

#include <memory>
#include <unordered_map>
#include "native/network_interface.hpp"
#include "response/status_response.hpp"
#include <shared_mutex>
#include "response/video_response.hpp"

using std::unique_ptr;
using std::unordered_map;
using tello::StatusResponse;
using tello::NetworkData;
using tello::VideoResponse;

namespace tello {

    class Command;
    class Response;
    class Network;

    using status_handler = void (*)(const StatusResponse&);
    using video_handler = void (*)(const VideoResponse& frame);

    class Tello {
    public:
        explicit Tello(ip_address telloIp);
        ~Tello();

        void setStatusHandler(status_handler statusHandler);
        void setVideoHandler(video_handler videoHandler);
        unique_ptr<Response> exec(const Command& command);
        [[nodiscard]] ip_address ip() const;

        friend class Network;

    private:
        static unordered_map<ip_address, const Tello*> _telloMapping;
        static std::shared_mutex _telloMappingMutex;

        static NetworkData mapToNetworkData(ip_address telloIp);

        const NetworkData _clientaddr;
        status_handler _statusHandler;
        video_handler _videoHandler;
    };
}