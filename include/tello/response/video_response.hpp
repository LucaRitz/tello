#pragma once
#include "../response.hpp"

#include <string>

using std::string;

namespace tello {

    class VideoResponse : public Response {
    public:
        VideoResponse(unsigned char* videoFrame, unsigned int length);
        VideoResponse(const VideoResponse& other);
        VideoResponse& operator=(const VideoResponse& other);
        VideoResponse(VideoResponse&& other) noexcept;
        VideoResponse& operator=(VideoResponse&& other) noexcept;
        ~VideoResponse();

        [[nodiscard]] unsigned char* videoFrame() const;
        [[nodiscard]] unsigned int length() const;

    private:
        unsigned char* _videoFrame;
        unsigned int _length;
    };
}