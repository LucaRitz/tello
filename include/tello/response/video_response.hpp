#pragma once
#include "../response.hpp"

#include <string>

using std::string;

namespace tello {

    class VideoResponse : public Response {
    public:
        explicit VideoResponse(string videoFrame);

        [[nodiscard]] const string& videoFrame() const;

    private:
        string _videoFrame;
    };
}