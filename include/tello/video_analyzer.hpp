#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace tello {

    class VideoAnalyzer {
    public:
        VideoAnalyzer() = default;
        VideoAnalyzer(const VideoAnalyzer&) = delete;
        VideoAnalyzer& operator=(const VideoAnalyzer&) = delete;
        VideoAnalyzer(VideoAnalyzer&&) = delete;
        VideoAnalyzer& operator=(VideoAnalyzer&&) = delete;

        bool append(const string& framePart);
        [[nodiscard]] string frame() const;
        void clean();

    private:
        string _frame;
    };
}