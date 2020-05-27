#include <tello/response/video_response.hpp>

tello::VideoResponse::VideoResponse(unsigned char* videoFrame, unsigned int length) :
        Response(Status::OK),
        _videoFrame(reinterpret_cast<unsigned char*>(std::memcpy(new unsigned char[length], videoFrame, length))),
        _length(length){}

tello::VideoResponse::VideoResponse(const VideoResponse& other) :
        Response(other),
        _length(other.length()) {
    delete[] _videoFrame;
    _videoFrame = reinterpret_cast<unsigned char*>(std::memcpy(new unsigned char[other.length()], other._videoFrame,
                                                             other.length()));
}

tello::VideoResponse& tello::VideoResponse::operator=(const VideoResponse& other) {
    if (this == &other) {
        return *this;
    }

    delete[] _videoFrame;
    _videoFrame = reinterpret_cast<unsigned char*>(std::memcpy(new unsigned char[other.length()], other._videoFrame,
                                                               other.length()));
    _length = other._length;
    return *this;
}

tello::VideoResponse::VideoResponse(VideoResponse&& other) noexcept :
    _videoFrame(other._videoFrame),
    _length(other._length) {
    other._videoFrame = nullptr;
    other._length = 0;
}

tello::VideoResponse& tello::VideoResponse::operator=(VideoResponse&& other) noexcept {
    this->_videoFrame = other._videoFrame;
    this->_length = other._length;

    other._videoFrame = nullptr;
    other._length = 0;
    return *this;
}

tello::VideoResponse::~VideoResponse() {
    delete[] _videoFrame;
    _videoFrame = nullptr;
}

unsigned char* tello::VideoResponse::videoFrame() const {
    return _videoFrame;
}

unsigned int tello::VideoResponse::length() const {
    return _length;
}