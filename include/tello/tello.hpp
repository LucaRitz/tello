#pragma once

#include <memory>
#include <unordered_map>
#include "native/network_interface.hpp"
#include "response/status_response.hpp"
#include <shared_mutex>
#include "response/video_response.hpp"
#include "tello_interface.hpp"

using std::shared_ptr;
using std::unordered_map;
using tello::StatusResponse;
using tello::NetworkData;
using tello::VideoResponse;

namespace tello {

    class Command;
    class Response;
    class Network;
    class QueryResponse;

    using status_handler = void (*)(const StatusResponse&);
    using video_handler = void (*)(const VideoResponse& frame);

    class Tello : public TelloInterface<shared_ptr<Response>, shared_ptr<QueryResponse>> {
    public:
        explicit Tello(ip_address telloIp);
        ~Tello();

        void setStatusHandler(status_handler statusHandler);
        void setVideoHandler(video_handler videoHandler);
        [[nodiscard]] ip_address ip() const;

        /////////////////////////////////////////////////////////////
        ///// COMMANDS //////////////////////////////////////////////
        /////////////////////////////////////////////////////////////

        [[nodiscard]] shared_ptr<Response> command() const override;
        [[nodiscard]] shared_ptr<Response> takeoff() const override;
        [[nodiscard]] shared_ptr<Response> land() const override;
    	
        [[nodiscard]] shared_ptr<Response> streamon() const override;
        [[nodiscard]] shared_ptr<Response> streamoff() const override;
    	
        [[nodiscard]] shared_ptr<Response> up(int x) const override;
        [[nodiscard]] shared_ptr<Response> down(int x) const override;
        [[nodiscard]] shared_ptr<Response> left(int x) const override;
        [[nodiscard]] shared_ptr<Response> right(int x) const override;
        [[nodiscard]] shared_ptr<Response> forward(int x) const override;
        [[nodiscard]] shared_ptr<Response> back(int x) const override;
    	
        [[nodiscard]] shared_ptr<Response> clockwise_turn(int x) const override;
        [[nodiscard]] shared_ptr<Response> counterclockwise_turn(int x) const override;

    	[[nodiscard]] shared_ptr<Response> flip(char flip_direction) const override;
    	
        [[nodiscard]] shared_ptr<Response> stop() const override;
        [[nodiscard]] shared_ptr<Response> emergency() const override;

        [[nodiscard]] shared_ptr<QueryResponse> wifi() const override;

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