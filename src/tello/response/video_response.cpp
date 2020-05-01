#include <tello/response/video_response.hpp>

tello::VideoResponse::VideoResponse(const char* videoFrame) :
        Response(Status::OK),
        _videoFrame(videoFrame) {}

const char* const tello::VideoResponse::videoFrame() const {
    return _videoFrame;
}