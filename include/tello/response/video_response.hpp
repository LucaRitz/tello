#pragma once
#include "../response.hpp"

namespace tello {

    class VideoResponse : public Response {
    public:
        explicit VideoResponse(const char* videoFrame);

        [[nodiscard]] const char* const videoFrame() const;

    private:
        const char* const _videoFrame;
    };
}