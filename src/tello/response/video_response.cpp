#include <tello/response/video_response.hpp>

tello::VideoResponse::VideoResponse(string videoFrame) :
        Response(Status::OK),
        _videoFrame(std::move(videoFrame)) {}

const string& tello::VideoResponse::videoFrame() const {
    return _videoFrame;
}