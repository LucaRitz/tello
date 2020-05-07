#pragma once

#include <memory>
#include <unordered_map>
#include "native/network_interface.hpp"
#include "response/status_response.hpp"
#include <shared_mutex>
#include "response/video_response.hpp"
#include "tello_interface.hpp"
#include <future>

using std::shared_ptr;
using std::unordered_map;
using tello::StatusResponse;
using tello::NetworkData;
using tello::VideoResponse;
using std::future;

namespace tello {

    class Command;
    class Response;
    class Network;
    class QueryResponse;

    using status_handler = void (*)(const StatusResponse&);
    using video_handler = void (*)(const VideoResponse& frame);

    class Tello : public TelloInterface<future<Response>, future<QueryResponse>> {
    public:
        explicit Tello(ip_address telloIp);
        ~Tello();

        void setStatusHandler(status_handler statusHandler);
        void setVideoHandler(video_handler videoHandler);
        [[nodiscard]] ip_address ip() const;

        /////////////////////////////////////////////////////////////
        ///// COMMANDS //////////////////////////////////////////////
        /////////////////////////////////////////////////////////////

        [[nodiscard]] future<Response> command() const override;
        [[nodiscard]] future<Response> takeoff() const override;
        [[nodiscard]] future<Response> land() const override;

        [[nodiscard]] future<Response> streamon() const override;
        [[nodiscard]] future<Response> streamoff() const override;

        [[nodiscard]] future<Response> up(int x) const override;
        [[nodiscard]] future<Response> down(int x) const override;
        [[nodiscard]] future<Response> left(int x) const override;
        [[nodiscard]] future<Response> right(int x) const override;
        [[nodiscard]] future<Response> forward(int x) const override;
        [[nodiscard]] future<Response> back(int x) const override;

        [[nodiscard]] future<Response> clockwise_turn(int x) const override;
        [[nodiscard]] future<Response> counterclockwise_turn(int x) const override;

    	[[nodiscard]] future<Response> flip(char flip_direction) const override;

        [[nodiscard]] future<Response> stop() const override;
        [[nodiscard]] future<Response> emergency() const override;

        [[nodiscard]] future<Response> set_speed(int velocity) const override;
        [[nodiscard]] future<Response> rc_control(int x, int y, int z, int r) const override;

        [[nodiscard]] future<QueryResponse> read_speed() const override;
        [[nodiscard]] future<QueryResponse> read_wifi() const override;

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