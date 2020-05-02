#pragma once

#include <memory>
#include <unordered_map>
#include "native/network_interface.hpp"
#include "response/status_response.hpp"
#include <shared_mutex>
#include "response/video_response.hpp"
#include "connection/command_strategy.hpp"
#include "tello_interface.hpp"

using std::unique_ptr;
using std::unordered_map;
using tello::StatusResponse;
using tello::NetworkData;
using tello::VideoResponse;
using tello::CommandStrategy;

namespace tello {

    class Command;
    class Response;
    class Network;
    class QueryResponse;

    using status_handler = void (*)(const StatusResponse&);
    using video_handler = void (*)(const VideoResponse& frame);

    class Tello : public TelloInterface<unique_ptr<Response>, unique_ptr<QueryResponse>> {
    public:
        explicit Tello(ip_address telloIp);
        ~Tello();

        void setStatusHandler(status_handler statusHandler);
        void setVideoHandler(video_handler videoHandler);
        [[nodiscard]] ip_address ip() const;

        /////////////////////////////////////////////////////////////
        ///// COMMANDS //////////////////////////////////////////////
        /////////////////////////////////////////////////////////////

        [[nodiscard]] unique_ptr<Response> command() const override;
        [[nodiscard]] unique_ptr<Response> takeoff() const override;
        [[nodiscard]] unique_ptr<Response> land() const override;
        [[nodiscard]] unique_ptr<Response> up(int x) const override;
        [[nodiscard]] unique_ptr<Response> streamon() const override;
        [[nodiscard]] unique_ptr<Response> streamoff() const override;
        [[nodiscard]] unique_ptr<Response> clockwise_turn(int x) const override;

        [[nodiscard]] unique_ptr<QueryResponse> wifi() const override;

        /////////////////////////////////////////////////////////////
        ///// END COMMANDS //////////////////////////////////////////
        /////////////////////////////////////////////////////////////

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