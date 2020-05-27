#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

using std::vector;
using std::unordered_map;
using std::string;
using std::shared_ptr;
using ip_address = unsigned long;

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
        [[nodiscard]] unsigned char* frame(ip_address address) const;
        [[nodiscard]] unsigned int length(ip_address address) const;
        void clean(ip_address address);

    private:

        unordered_map<ip_address, shared_ptr<vector<NetworkResponse>>> _frames;

        static bool isStart(const char* const& framePart, int length);
    };
}