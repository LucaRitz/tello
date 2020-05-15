#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace tello {

    class VideoAnalyzer {
    public:
        VideoAnalyzer();
        VideoAnalyzer(const VideoAnalyzer&) = delete;
        VideoAnalyzer& operator=(const VideoAnalyzer&) = delete;
        VideoAnalyzer(VideoAnalyzer&&) = delete;
        VideoAnalyzer& operator=(VideoAnalyzer&&) = delete;
        ~VideoAnalyzer();

        bool append(const unsigned char* const& framePart, int length);
        [[nodiscard]] unsigned char* frame() const;
        [[nodiscard]] unsigned int length() const;
        void clean();

    private:
        unsigned char* _frame;
        unsigned int _currentSize;

        string string_to_hex(unsigned char*& input, unsigned int size);
    };
}