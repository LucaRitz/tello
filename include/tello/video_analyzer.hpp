#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace tello {

    class NetworkResponse;

    class VideoAnalyzer {
    public:
        VideoAnalyzer();
        VideoAnalyzer(const VideoAnalyzer&) = delete;
        VideoAnalyzer& operator=(const VideoAnalyzer&) = delete;
        VideoAnalyzer(VideoAnalyzer&&) = delete;
        VideoAnalyzer& operator=(VideoAnalyzer&&) = delete;
        ~VideoAnalyzer();

        bool append(NetworkResponse& response);
        [[nodiscard]] unsigned char* frame() const;
        [[nodiscard]] unsigned int length() const;
        void clean();

    private:
        vector<NetworkResponse> _frames;
        vector<int> _frameSizes;

        static bool isStart(const char* const& framePart, int length);
    };
}